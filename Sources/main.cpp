/*
 * This file is part of ExpertSDR
 *
 * ExpertSDR is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 *
 * ExpertSDR is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with GNU Radio; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 *
 *
 * Copyright (C) 2012 Valery Mikhaylovsky
 * The authors can be reached by email at maksimus1210@gmail.com
 */

#include "ExpertSDR_vA2_1.h"

#include <QtGui>
#include <QApplication>
#include "Logger/VLogger.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
    qInstallMsgHandler(logMassage);
	qDebug("Log-clear");

	if(!QGLFormat::hasOpenGL() || !(QGLFormat::openGLVersionFlags() & QGLFormat::OpenGL_Version_1_2))
	{
		QMessageBox::information(0, "OpenGL 1.2 ERROR!", "This system does not support OpenGL 1.2!");
		qCritical() << "qApp: system does not support OpenGL 1.2!";
		return -1;
	}

	QTranslator translator(0);
	if(translator.load( QString("ExpertSDR_ru"), "." ))
		a.installTranslator(&translator);
	else
		qWarning() << "qApp: set translation file - file is not exist!";

	QPixmap pixmap(":images/splash.png");
	QSplashScreen splash(pixmap);
	splash.show();
	ExpertSDR_vA2_1 pw;
	pw.show();

	splash.finish(&pw);
	return a.exec();
}
