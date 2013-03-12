

#pragma once

// =========================================

#include <QWidget>
#include <QList>
#include <QVariant>

class QAbstractItemView;
class QListWidgetItem;
class QListWidget;
class QPushButton;
class QSettings;

// =========================================

namespace sciAppFramework
{

  // =========================================

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

    protected:
      template <class V> V* itemViewTemplateCast() { return dynamic_cast<V*>(ItemView); }
      template <class V> const V* itemViewTemplateCast() const { return dynamic_cast<const V*>(ItemView); }

    protected:
      virtual void emitSelectionChanged();
      virtual void enableSelectionSignalItemView() = 0;
      virtual void disableSelectionSignalItemView() = 0;

      void init();

    public:
      catalogueWidget( QWidget *Parent = NULL );

      virtual void loadSettings( QSettings *Settings );
      virtual void saveSettings( QSettings *Settings );

    signals:
      void selectionChanged();
  };
  
  // =========================================
  
  template <class itemView> class catalogueItemViewTemplateWidget : public catalogueWidget
  {
    protected:
      void enableSelectionSignalItemView()  { itemViewCast()->disconnect(); }
      void disableSelectionSignalItemView() { connect( itemViewCast(), SIGNAL( itemSelectionChanged() ), SIGNAL(selectionChanged()) ); }

    protected:
      itemView* itemViewCast() { return itemViewTemplateCast<itemView>(); }
      const itemView* itemViewCast() const { return itemViewTemplateCast<itemView>(); }
      
      bool isItemViewInited() const { return itemViewCast() != NULL; }

    public:
      catalogueItemViewTemplateWidget( QWidget *Parent = NULL ) : catalogueWidget(Parent) {}
      
      int count() const { return itemViewCast()->count(); }
  };
  
  // =========================================
  
  class catalogueListWidget : public catalogueItemViewTemplateWidget<QListWidget>
  {
    private:
      QAbstractItemView* createItemView();

    public:
      catalogueListWidget( QWidget *Parent = NULL );

      QListWidgetItem* add( const QString &Title ) { return add(Title,QVariant(),true); }
      QListWidgetItem* add( const QString &Title, bool Selected ) { return add(Title,QVariant(),Selected); }
      QListWidgetItem* add( const QString &Title, const QVariant &Data, bool Selected );
      void clear();

      bool isSelected( unsigned Index ) const;
      bool isSelected( const QVariant &Data ) const;
      bool isSelected( const QListWidgetItem* Item ) const;
  };
  
  // =========================================

}

