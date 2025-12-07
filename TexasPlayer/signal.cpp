#include "signal.h"
#include <qstring.h>
#include <qdir.h>
#include <QLockFile>
bool signal::setupSignalHandlers()
{
    QString lockPath = QDir::tempPath() + "/my_app_instance.lock";

    // 2. 创建静态对象（必须是静态或持久存在的，否则函数结束锁就释放了）
    static QLockFile lockFile(lockPath);

    // 3. 尝试加锁
    // tryLock() 参数是等待时间，0 表示立即返回，不阻塞
    if (!lockFile.tryLock(100)) {
        // 加锁失败，说明已有实例在运行
        return false;
    }

    // 加锁成功，程序继续
    return true;
}