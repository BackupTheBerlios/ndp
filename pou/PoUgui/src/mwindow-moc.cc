/****************************************************************************
** MainWindow meta object code from reading C++ file 'mwindow.h'
**
** Created: Wed Apr 7 20:00:28 2004
**      by: The Qt MOC ($Id: mwindow-moc.cc,v 1.2 2004/04/07 18:23:17 ob821 Exp $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "mwindow.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.2.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *MainWindow::className() const
{
    return "MainWindow";
}

QMetaObject *MainWindow::metaObj = 0;
static QMetaObjectCleanUp cleanUp_MainWindow( "MainWindow", &MainWindow::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString MainWindow::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "MainWindow", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString MainWindow::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "MainWindow", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* MainWindow::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QMainWindow::staticMetaObject();
    static const QUMethod slot_0 = {"MenuFileOpen", 0, 0 };
    static const QUMethod slot_1 = {"MenuFileClose", 0, 0 };
    static const QUMethod slot_2 = {"MenuFileExit", 0, 0 };
    static const QUMethod slot_3 = {"MenuSettingsArgs", 0, 0 };
    static const QUMethod slot_4 = {"MenuWindowsNew", 0, 0 };
    static const QUMethod slot_5 = {"MenuRenderingRender", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "MenuFileOpen()", &slot_0, QMetaData::Private },
	{ "MenuFileClose()", &slot_1, QMetaData::Private },
	{ "MenuFileExit()", &slot_2, QMetaData::Private },
	{ "MenuSettingsArgs()", &slot_3, QMetaData::Private },
	{ "MenuWindowsNew()", &slot_4, QMetaData::Private },
	{ "MenuRenderingRender()", &slot_5, QMetaData::Private }
    };
    metaObj = QMetaObject::new_metaobject(
	"MainWindow", parentObject,
	slot_tbl, 6,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_MainWindow.setMetaObject( metaObj );
    return metaObj;
}

void* MainWindow::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "MainWindow" ) )
	return this;
    return QMainWindow::qt_cast( clname );
}

bool MainWindow::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: MenuFileOpen(); break;
    case 1: MenuFileClose(); break;
    case 2: MenuFileExit(); break;
    case 3: MenuSettingsArgs(); break;
    case 4: MenuWindowsNew(); break;
    case 5: MenuRenderingRender(); break;
    default:
	return QMainWindow::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool MainWindow::qt_emit( int _id, QUObject* _o )
{
    return QMainWindow::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool MainWindow::qt_property( int id, int f, QVariant* v)
{
    return QMainWindow::qt_property( id, f, v);
}

bool MainWindow::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
