#include "qt/QtApplication.h"

#include <QFileOpenEvent>

#include "utility/file/FilePath.h"
#include "utility/messaging/type/MessageLoadProject.h"
#include "utility/messaging/type/MessageWindowFocus.h"
#include "utility/logging/LogManager.h"
#include "utility/utilityApp.h"

QtApplication::QtApplication(int& argc, char** argv)
	: QApplication(argc, argv)
{
	connect(this, &QGuiApplication::applicationStateChanged, this, &QtApplication::onApplicationStateChanged);
}

int QtApplication::exec()
{
	return QApplication::exec();
}

// responds to FileOpenEvent specific for Mac
bool QtApplication::event(QEvent *event)
{
	if (event->type() == QEvent::FileOpen)
	{
		QFileOpenEvent* fileEvent = dynamic_cast<QFileOpenEvent*>(event);

		FilePath path(fileEvent->file().toStdString());

		if (path.exists() && (path.extension() == ".srctrlprj" || path.extension() == ".coatiproject"))
		{
			MessageLoadProject(path, false).dispatch();
			return true;
		}
	}

	return QApplication::event(event);
}

void QtApplication::onApplicationStateChanged(Qt::ApplicationState state)
{
	MessageWindowFocus(state == Qt::ApplicationActive).dispatch();
}
