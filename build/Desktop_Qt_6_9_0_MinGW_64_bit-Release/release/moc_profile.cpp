/****************************************************************************
** Meta object code from reading C++ file 'profile.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.9.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../src/ui/profile.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'profile.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 69
#error "This file was generated using the moc from 6.9.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
QT_WARNING_DISABLE_GCC("-Wuseless-cast")
namespace {
struct qt_meta_tag_ZN15ProfileEditPageE_t {};
} // unnamed namespace

template <> constexpr inline auto ProfileEditPage::qt_create_metaobjectdata<qt_meta_tag_ZN15ProfileEditPageE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "ProfileEditPage",
        "onHelpClicked",
        "",
        "onVoteClicked",
        "onMyVotesClicked",
        "onCreatePollClicked",
        "onMyPollsClicked",
        "onProfileClicked"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'onHelpClicked'
        QtMocHelpers::SignalData<void()>(1, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'onVoteClicked'
        QtMocHelpers::SignalData<void()>(3, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'onMyVotesClicked'
        QtMocHelpers::SignalData<void()>(4, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'onCreatePollClicked'
        QtMocHelpers::SignalData<void()>(5, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'onMyPollsClicked'
        QtMocHelpers::SignalData<void()>(6, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'onProfileClicked'
        QtMocHelpers::SignalData<void()>(7, 2, QMC::AccessPublic, QMetaType::Void),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<ProfileEditPage, qt_meta_tag_ZN15ProfileEditPageE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject ProfileEditPage::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN15ProfileEditPageE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN15ProfileEditPageE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN15ProfileEditPageE_t>.metaTypes,
    nullptr
} };

void ProfileEditPage::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<ProfileEditPage *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->onHelpClicked(); break;
        case 1: _t->onVoteClicked(); break;
        case 2: _t->onMyVotesClicked(); break;
        case 3: _t->onCreatePollClicked(); break;
        case 4: _t->onMyPollsClicked(); break;
        case 5: _t->onProfileClicked(); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (ProfileEditPage::*)()>(_a, &ProfileEditPage::onHelpClicked, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (ProfileEditPage::*)()>(_a, &ProfileEditPage::onVoteClicked, 1))
            return;
        if (QtMocHelpers::indexOfMethod<void (ProfileEditPage::*)()>(_a, &ProfileEditPage::onMyVotesClicked, 2))
            return;
        if (QtMocHelpers::indexOfMethod<void (ProfileEditPage::*)()>(_a, &ProfileEditPage::onCreatePollClicked, 3))
            return;
        if (QtMocHelpers::indexOfMethod<void (ProfileEditPage::*)()>(_a, &ProfileEditPage::onMyPollsClicked, 4))
            return;
        if (QtMocHelpers::indexOfMethod<void (ProfileEditPage::*)()>(_a, &ProfileEditPage::onProfileClicked, 5))
            return;
    }
}

const QMetaObject *ProfileEditPage::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ProfileEditPage::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN15ProfileEditPageE_t>.strings))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int ProfileEditPage::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 6)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 6;
    }
    return _id;
}

// SIGNAL 0
void ProfileEditPage::onHelpClicked()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void ProfileEditPage::onVoteClicked()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void ProfileEditPage::onMyVotesClicked()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void ProfileEditPage::onCreatePollClicked()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}

// SIGNAL 4
void ProfileEditPage::onMyPollsClicked()
{
    QMetaObject::activate(this, &staticMetaObject, 4, nullptr);
}

// SIGNAL 5
void ProfileEditPage::onProfileClicked()
{
    QMetaObject::activate(this, &staticMetaObject, 5, nullptr);
}
QT_WARNING_POP
