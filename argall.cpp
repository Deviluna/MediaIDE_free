#include "argall.h"
#include <QDesktopServices>

ArgAll::ArgAll()
{
// 全局调用的参数都在这里设置，但是目前开发先不重构了



}
QString ArgAll::documentPath(){

    return QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation).replace("/","\\");
}
QString ArgAll::configPath(){

    return  QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation).replace("/","\\")+"\\MediaIDEFile";

}
