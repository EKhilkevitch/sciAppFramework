
#include "sciAppFramework/settingsStorage.h"

#include <QString>
#include <QStringList>
#include <QDebug>

#include <cstdlib>

using namespace sciAppFramework;

int ErrorsCount = 0;

template <class T, class U> bool assertEqImpl( const T &A, const U &B, int Line, const char *StrA, const char *StrB )
{
  if ( A != B )
  {
    qDebug() << "Assertion failed at line " << Line;
    qDebug() << "Expected: " << StrA << " = " << A;
    qDebug() << "Actual: " << StrB << " = " << B;
    ++ErrorsCount;
    return false;
  }
  return true;
}

#define EXPECT_EQ( A, B )                               \
  do {                                                  \
    assertEqImpl( (A), (B), __LINE__, #A, #B );         \
  } while (false)

#define ASSERT_EQ( A, B )                               \
  do {                                                  \
    bool _Ok = assertEqImpl( (A), (B), __LINE__, #A, #B );         \
    if ( !_Ok )                                          \
      std::exit(1);                                     \
  } while (false)

int main( int argc, char **argv )
{

  QString Xml;

  {
    xmlSettingsStorage Storage;
    
    Storage.setValue( "Key1", "Value1" );
    EXPECT_EQ( "Value1", Storage.value("Key1") );
    EXPECT_EQ( QVariant(), Storage.value("Key2") );
    
    Storage.setValue( "Key2", "Value2" );
    EXPECT_EQ( "Value1", Storage.value("Key1") );
    EXPECT_EQ( "Value2", Storage.value("Key2") );

    Storage.beginGroup( "Group1" );
    Storage.setValue( "Key3", "Value3" );
    Storage.setValue( "Key4", 7 );
    EXPECT_EQ( QVariant(), Storage.value("Key1") );
    EXPECT_EQ( "Value3", Storage.value("Key3") );
    EXPECT_EQ( 7, Storage.value("Key4") );
    Storage.endGroup();
    
    Storage.beginGroup( "Group2" );
    Storage.setValue( "Key5", 8.4 );
    EXPECT_EQ( QVariant(), Storage.value("Key1") );
    EXPECT_EQ( QVariant(), Storage.value("Key3") );
    EXPECT_EQ( 8.4, Storage.value("Key5") );
    Storage.endGroup();

    Xml = Storage.toXml();
  }

  qDebug() << "XML: " << Xml;

  {
    xmlSettingsStorage Storage;
    bool Ok = Storage.fromXml(Xml);
    ASSERT_EQ( true, Ok );

    EXPECT_EQ( "Value1", Storage.value("Key1") );
    EXPECT_EQ( "Value2", Storage.value("Key2") );
    Storage.beginGroup( "Group1" );
    EXPECT_EQ( "Value3", Storage.value("Key3") );
    EXPECT_EQ( 7, Storage.value("Key4") );
    Storage.endGroup();
    Storage.beginGroup( "Group2" );
    EXPECT_EQ( 8.4, Storage.value("Key5") );
    Storage.endGroup();
  }

  if ( ErrorsCount == 0 )
  {
    qDebug() << "SUCCESS";
  } else {
    qDebug() << "FAILED: " << ErrorsCount << " times";
  }

  return 0;
}

