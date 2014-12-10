
#include <QtCore>
#include <QtGui>

#include "sciAppFramework/inputWidget.h"
#include "sciAppFramework/multiInputWidget.h"
#include "sciAppFramework/multiInputXmlFactory.h"

using namespace sciAppFramework;

struct testInputWidget : public multiInputWidget
{
  QSettings* createSettings() { return new QSettings("./testMultiInputWidgetXml.conf",QSettings::IniFormat); }
  void loadSettings()
  { 
    QSettings *Settings = createSettings(); 
    multiInputWidget::loadSettings( Settings ); 
    delete Settings;
  }

  ~testInputWidget()  
  { 
    qDebug() << "Save settings";
    QSettings *Settings = createSettings(); 
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
  return app.exec(); 
}


