#include "ProcessControl.h"
#include "BBInterfaceStatus.h"
#include <QtDebug>

// --------------------------------------------------------------------------
//
// Function
//      Name:    BBInterfaceStatus::BBInterfaceStatus
//      Purpose: Constructor
//      Created: 26/6/08
//
// ------------------

BBInterfaceStatus::BBInterfaceStatus(ABConfiguration * myconf)
{
    qDebug() << "BBInterfaceStatus::BBInterfaceStatus(): begin";
    conf = myconf;
    pc = new ProcessControl(conf);
    ril = new BBRunningInstanceList(conf, pc);
    QObject::connect(ril, SIGNAL(new_event(Event)), this, SLOT(onBBRunningInstanceListEvent(Event)));
}

BBInterfaceStatus::~BBInterfaceStatus()
{
}


// --------------------------------------------------------------------------
//
// Function
//      Name:    bool BBInterfaceStatus::CIIRestart
//      Purpose: Reinicia la instancia actual si la hubiere
//      Created: 20/6/08
//
// ------------------
//
bool BBInterfaceStatus::CIIRestart()
{
    qDebug() << "BBInterfaceStatus::CIIRestart(): begin";
    //if (currentInterface.get() == NULL) return false;
    //if (currentInterface->status() == OFF) return CIIStart();
    //return ProcessController->restart(currentInterface->instance);
}

// --------------------------------------------------------------------------
//
// Function
//      Name:    bool BBInterfaceStatus::beginProcedure
//      Purpose: Arranca la instancia actual
//      Created: 20/6/08
//
// ------------------
bool BBInterfaceStatus::CIIStart()
{
    qDebug() << "BBInterfaceStatus::CIIStart(): begin";
    //if (currentInterface.get() == NULL) return false;
    //if (currentInterface->status() == OK) return true;
    //return ProcessController->startInstance(currentInterface->instance);
}



// --------------------------------------------------------------------------
//
// Function
//      Name:    bool BBInterfaceStatus::CIIStop
//      Purpose: Para la instancia actual si la hubiere
//      Created: 20/6/08
//
// ------------------
bool BBInterfaceStatus::CIIStop()
{
    qDebug() << "BBInterfaceStatus::CIIStop(): begin";
    //if (currentInterface.get() == NULL) return false;
    //if (currentInterface->status() == OFF) return true;
    //return ProcessController->stop(currentInterface->instance);
}


// --------------------------------------------------------------------------
//
// Function
//      Name:    bool BBInterfaceStatus::beginProcedure
//      Purpose: Procedimiento de arranque. Intenta conectar a la instancia correcta, o arrancarla
//      Created: 20/6/08
//
// ------------------
//
bool BBInterfaceStatus::beginProcedure()
{
    return ril->beginProcedure();
}

// --------------------------------------------------------------------------
//
// Function
//      Name:    bool BBInterfaceStatus::connectToInstance
//      Purpose: conecta a la instancia pasada
//      Created: 20/6/08
//
// ------------------
bool BBInterfaceStatus::connectToInstance(BBInstance myinstance)
{
    qDebug() << "BBInterfaceStatus::connectToInstance(): begin";
    //Comprobar que es valida la instancia
    //if (!ProcessControl::isValid(myinstance)) return false;
    ////if (!isRunningInstance(myinstance)) return false; //TODO: Ver si esta comprobación es conveniente
    /*
    if (currentInterface->instance != myinstance) //TODO: operador !=
    {
        currentInterface.get(new BBBInterfaceSystem(myinstance))
    }
    */
    //TODO: Falta cambiar el estado
}

// --------------------------------------------------------------------------
//
// Function
//      Name:    bool BBInterfaceStatus::isRunningInstance
//      Purpose: Verifica si la instancia pasada esta en ejecución
//      Created: 20/6/08
//
// ------------------
bool BBInterfaceStatus::isRunningInstance(BBInstance myinstance)
{
    qDebug() << "BBInterfaceStatus::isRunningInstance(): begin";
    return ProcessControl::isRunningInstance(myinstance);
}

// --------------------------------------------------------------------------
//
// Function
//      Name:    bool BBInterfaceStatus::launchInstance
//      Purpose: Asigna la instancia pasada a la actual y la intenta arrancar
//      Created: 20/6/08
//
// ------------------

bool BBInterfaceStatus::launchInstance(BBInstance myinstance)
{
    qDebug() << "BBInterfaceStatus::launchInstance(): begin";
    //currentInterface.get(new BBBInterface(BBInstance));
    return CIIStart();
}


// --------------------------------------------------------------------------
//
// Function
//      Name:    std::list<BBInstance> BBInterfaceStatus::allValidInstances
//      Purpose: Devuelve una lista con todas las instancias cuya configuracion es la misma que la indicada en las preferencias
//      Created: 20/6/08
//
// ------------------
std::list<BBInstance> BBInterfaceStatus::allValidInstances()
{
    qDebug() << "BBInterfaceStatus::allValidInstances(): begin";
    return pc->allValidInstances();
}

// --------------------------------------------------------------------------
//
// Function
//      Name:    std::list<BBInstance> BBInterfaceStatus::allInstances()
//      Purpose: Devuelve una lista con todas las instancias en el sistema de bbackupd
//      Created: 20/6/08
//
// ------------------
std::list<BBInstance> BBInterfaceStatus::allInstances()
{
    qDebug() << "BBInterfaceStatus::allInstances(): begin";
//    return ProcessControl::allInstances();
    std::list<BBInstance> tmp;
    return tmp; //TODO
}



// --------------------------------------------------------------------------
//
// Function
//      Name:    BBInstance BBInterfaceStatus::currentInstance()
//      Purpose: Devuelve el valor de la instancia actual
//      Created: 26/6/08
//
// ------------------
BBInstance BBInterfaceStatus::currentInstance() const
{
    qDebug() << "BBInterfaceStatus::currentInstance(): begin";
    return ril->currentInstance();
}

void BBInterfaceStatus::onBBRunningInstanceListEvent(Event myevent)
{
    qDebug() << "BBInterfaceStatus::onBBRunningInstanceListEvent:Begin";
    emit new_event(myevent);
}


bool BBInterfaceStatus::haltCurrentInstance()
{
    return ProcessControl::haltInstance(currentInstance());
}
