

#pragma once

// ======================================================

#include <QWidget>
#include <QList>
#include <QVariant>

#include <QListWidget>
#include <QTableWidget>

class QAbstractItemView;
class QListWidgetItem;
class QListWidget;
class QPushButton;
class QSettings;

// ======================================================

namespace sciAppFramework
{

  // ======================================================

  class catalogueWidget : public QWidget
  {
    Q_OBJECT

    private:
      QWidget *ButtonsWidget;
      QAbstractItemView *ItemView;

    private:
      virtual QAbstractItemView* createItemView() = 0;
      
      virtual QWidget* createButtonsWidget();
      virtual QList<QPushButton*> createButtonsList();

      virtual bool putButtonsonTheTop() const { return true; }
      virtual QAbstractItemView::SelectionMode viewSelectionMode() const { return QAbstractItemView::SingleSelection; }

    protected:
      void keyPressEvent( QKeyEvent *Event );

    protected:
      template <class V> V* itemViewTemplateCast() { return dynamic_cast<V*>(ItemView); }
      template <class V> const V* itemViewTemplateCast() const { return dynamic_cast<const V*>(ItemView); }

    protected:
      virtual void emitSelectionChanged();
      virtual void enableSelectionSignalItemView() = 0;
      virtual void disableSelectionSignalItemView() = 0;

      void initWidget();

    public:
      catalogueWidget( QWidget *Parent = NULL );
      ~catalogueWidget();

    signals:
      void selectionChanged();
      void deleteCurrentItem();
  };
  
  // ======================================================
  
  template <class itemView, class itemWidget> class catalogueItemViewTemplateWidget : public catalogueWidget
  {
    protected:
      void enableSelectionSignalItemView();
      void disableSelectionSignalItemView();

    protected:
      itemView* itemViewCast() { return itemViewTemplateCast<itemView>(); }
      const itemView* itemViewCast() const { return itemViewTemplateCast<itemView>(); }
      
      bool isItemViewInited() const { return itemViewCast() != NULL; }

    public:
      catalogueItemViewTemplateWidget( QWidget *Parent = NULL ) : 
        catalogueWidget(Parent) {}
      
      int count() const { return itemViewCast()->count(); }
      QList<itemWidget*> selectedItems() { return itemViewCast()->selectedItems(); }
  };
  
  // ======================================================
  
  class catalogueListWidget : public catalogueItemViewTemplateWidget<QListWidget,QListWidgetItem>
  {
    Q_OBJECT

    private:
      QListWidget* createItemView();

    public:
      catalogueListWidget( QWidget *Parent = NULL );
      virtual ~catalogueListWidget() = 0;

      QListWidgetItem* add( const QString &Title ) { return add(Title,QVariant(),true); }
      QListWidgetItem* add( const QString &Title, bool Selected ) { return add(Title,QVariant(),Selected); }
      QListWidgetItem* add( const QString &Title, const QVariant &Data, bool Selected );
      void clear();
      
      QListWidgetItem* item( int Row ) const;

      bool isSelected( unsigned Index ) const;
      bool isSelected( const QVariant &Data ) const;
      bool isSelected( const QListWidgetItem* Item ) const;
  };
  
  // ======================================================
  
  class catalogueTableWidget : public catalogueItemViewTemplateWidget<QTableWidget,QTableWidgetItem>
  {
    Q_OBJECT

    private:
      QTableWidget* createItemView();
      
    protected:
      void enableSelectionSignalItemView();

    public:
      catalogueTableWidget( QWidget *Parent = NULL );
      virtual ~catalogueTableWidget() = 0;

      void setColumnCount( int C ) { itemViewCast()->setColumnCount(C); }
      int columnCount() const { return itemViewCast()->columnCount(); }
      void setColumnWidth( int C, int W ) { itemViewCast()->setColumnWidth(C,W); }

      void setRowCount( int R ) { itemViewCast()->setRowCount(R); }
      int rowCount() const { return itemViewCast()->rowCount(); }
      void setRowHeight( int R, int H ) { itemViewCast()->setRowHeight(R,H); }

      QHeaderView* horizontalHeader() const { return itemViewCast()->horizontalHeader(); }
      QHeaderView* verticalHeader() const { return itemViewCast()->verticalHeader(); }

      void incrementRowCoumt() { setRowCount( rowCount() + 1 ); }

      QTableWidgetItem* setItem( int Row, int Column, const QString& Title, const QVariant& Data = QVariant() );
      QTableWidgetItem* setItem( int Row, int Column, QTableWidgetItem *Item );
      QTableWidgetItem* item( int Row, int Column ) const { return itemViewCast()->item(Row,Column); }
      QTableWidgetItem* takeItem( int Row, int Column ) { return itemViewCast()->takeItem(Row,Column); }

    signals:
      void currentChanged( QTableWidgetItem*, QTableWidgetItem* );
      void cellClicked( int Row, int Column );
      void cellChanged( int Row, int Column );
      void cellEntered( int Row, int Column );
  };
  
  // ======================================================
      
  template <class itemView, class itemWidget> void catalogueItemViewTemplateWidget<itemView,itemWidget>::enableSelectionSignalItemView()  
  { 
    connect( itemViewCast(), SIGNAL( itemSelectionChanged() ), SIGNAL(selectionChanged()) ); 
  }
  
  // ------------------------------------------------------

  template <class itemView, class itemWidget> void catalogueItemViewTemplateWidget<itemView,itemWidget>::disableSelectionSignalItemView() 
  { 
    itemViewCast()->disconnect();  
  }
  
  // ======================================================

}

