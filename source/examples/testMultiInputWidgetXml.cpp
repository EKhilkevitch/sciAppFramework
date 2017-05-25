
#include "sciAppFramework/inputWidget.h"
#include "sciAppFramework/multiInputWidget.h"
#include "sciAppFramework/multiInputXmlFactory.h"
#include "sciAppFramework/settingsStorage.h"

#include <QApplication>
#include <QDebug>

using namespace sciAppFramework;

struct testInputWidget : public multiInputWidget
{
  settingsStorage* createSettings() { return new fileSettingsStorage("./testMultiInputWidgetXml.conf"); }
  void loadSettings()
  { 
    settingsStorage *Settings = createSettings(); 
    multiInputWidget::loadSettings( Settings ); 
    delete Settings;
  }

  ~testInputWidget()  
  { 
    qDebug() << "Save settings";
    settingsStorage *Settings = createSettings(); 
    multiInputWidget::saveSettings( Settings ); 
    delete Settings;
  }
};

const QString Xml = 
"<testInput>\n"
"  <label name=\"LabelName\"><text>Simple label text</text><text>Next label text</text></label>\n"
"  <edit name=\"EditName\"><label>Edit Label</label><value>Edit value</value><stretch><input>5</input><label>5</label></stretch></edit>\n"
"  <spin name=\"SpinName\"><label>Spin</label><value>145</value><min>-4</min><max>200</max></spin>\n"
"  <spin name=\"SpinName2\"><label>Spin 2</label><value>100</value></spin>\n"
"  <doubleSpin name=\"DoubleSpinName\"><label>Double spin</label><value>4.5</value><min>0.5</min><max>1.5</max><step>0.1</step></doubleSpin>\n"
"  <double name=\"DoubleName\"><label>Double edit</label><min>2.5</min><max>10.2e6</max></double>\n"
"  <path name=\"Path\"><label>Save dialog</label><mode>save</mode></path>\n"
"  <path name=\"PathDir\"><label>Directory dialog</label><mode>opendir</mode></path>\n"
"  <spacing><value>15</value></spacing>\n"
"  <label><text>label Text1</text></label>\n"
"  <label><text>label Text2</text></label>\n"
"  <tab name=\"tab1\" label=\"Tab page1\">"
"  <edit name=\"tabedit1\"><label>Tab edit 1</label></edit>\n"
"  <label><text>Tab label text 1</text></label>\n"
"  </tab>\n"
"  <tab name=\"tab2\" label=\"Tab page2\">"
"  <label><text>Tab label text 2</text></label>\n"
"  <edit name=\"tabedit2\"><label>Tab edit 2</label></edit>\n"
"  </tab>\n"
"  <box name=\"box1\" label=\"Box 1\">\n"
"  <edit name=\"boxedit1\"><label>Box edit 1</label></edit>\n"
"  <edit name=\"boxedit2\"><label>Box edit 2</label></edit>\n"
"  </box>\n"
"  <combobox name=\"cbox1\" sname=\"cbox\" slabel=\"Combo box selector\" label=\"Item 1\">\n"
"  <edit name=\"cboxedit1\"><label>Combo box edit 1</label></edit>\n"
"  <edit name=\"cboxedit2\"><label>Combo box edit 2</label></edit>\n"
"  </combobox>\n"
"  <combobox name=\"cbox1\" sname=\"cbox\" slabel=\"Combo box selector\" label=\"Item 2\">\n"
"  <edit name=\"cboxedit3\"><label>Combo box edit 3</label></edit>\n"
"  </combobox>\n"
"  <combo name=\"ComboName\"><label>Combo</label><item data=\"1\">First item</item><item data=\"2\">Second item</item></combo>\n"
"  <check name=\"CheckName\"><label>Check</label><value>0</value></check>"
"</testInput>\n"
;

int main( int argc, char **argv )
{
  QApplication app(argc,argv);
  
  testInputWidget Widget;

  multiInputWidgetXmlFactory Factory( Xml );
  Factory.addItemsToMultiInputWidget( &Widget );

  Widget.loadSettings();
  Widget.show();

  qDebug() << "Map: " << Widget.stringValues();
  
  return app.exec(); 
}

