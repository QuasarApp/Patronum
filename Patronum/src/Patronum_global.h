/*
 * Copyright (C) 2018-2020 QuasarApp.
 * Distributed under the lgplv3 software license, see the accompanying
 * Everyone is permitted to copy and distribute verbatim copies
 * of this license document, but changing it is not allowed.
*/

#ifndef PATRONUM_GLOBAL_H
#define PATRONUM_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(PATRONUM_LIBRARY)
#  define PATRONUM_LIBRARYSHARED_EXPORT Q_DECL_EXPORT
#else
#  define PATRONUM_LIBRARYSHARED_EXPORT Q_DECL_IMPORT
#endif


#endif // PATRONUM_GLOBAL_H
