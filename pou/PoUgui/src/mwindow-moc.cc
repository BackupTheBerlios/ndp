/****************************************************************************
** MWindow meta object code from reading C++ file 'mwindow.h'
**
** Created: Fri Mar 12 22:12:01 2004
**      by: The Qt MOC ($Id: mwindow-moc.cc,v 1.1 2004/03/17 08:26:07 pumpkins Exp $)
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

const char *MWindow::className() const
{
    return "MWindow";
}

QMetaObject *MWindow::metaObj = 0;
static QMetaObjectCleanUp cleanUp_MWindow( "MWindow", &MWindow::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString MWindow::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "MWindow", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString MWindow::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "MWindow", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* MWindow::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QMainWindow::staticMetaObject();
    static const QUMethod slot_0 = {"menu_file_open", 0, 0 };
    static const QUMethod slot_1 = {"menu_file_close", 0, 0 };
    static const QUMethod slot_2 = {"menu_file_exit", 0, 0 };
    static const QUMethod slot_3 = {"menu_settings_args", 0, 0 };
    static const QUMethod slot_4 = {"menu_windows_new", 0, 0 };
    static const QUMethod slot_5 = {"menu_rendering_render", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "menu_file_open()", &slot_0, QMetaData::Private },
	{ "menu_file_close()", &slot_1, QMetaData::Private },
	{ "menu_file_exit()", &slot_2, QMetaData::Private },
	{ "menu_settings_args()", &slot_3, QMetaData::Private },
	{ "menu_windows_new()", &slot_4, QMetaData::Private },
	{ "menu_rendering_render()", &slot_5, QMetaData::Private }
    };
    metaObj = QMetaObject::new_metaobject(
	"MWindow", parentObject,
	slot_tbl, 6,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_MWindow.setMetaObject( metaObj );
    return metaObj;
}

void* MWindow::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "MWindow" ) )
	return this;
    return QMainWindow::qt_cast( clname );
}

bool MWindow::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: menu_file_open(); break;
    case 1: menu_file_close(); break;
    case 2: menu_file_exit(); break;
    case 3: menu_settings_args(); break;
    case 4: menu_windows_new(); break;
    case 5: menu_rendering_render(); break;
    default:
	return QMainWindow::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool MWindow::qt_emit( int _id, QUObject* _o )
{
    return QMainWindow::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool MWindow::qt_property( int id, int f, QVariant* v)
{
    return QMainWindow::qt_property( id, f, v);
}

bool MWindow::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
