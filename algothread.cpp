#include "algothread.h"
#include "maintenance.h"
#include "habitant.h"

AlgoThread* AlgoThread::algoThread = nullptr;

AlgoThread::AlgoThread(CityWidget* mainWindow,int _nbSite,int _nbHabitants,int _nbBorne,int _nbVelo) : displayQDebug(false), mutexQDebug()
{
    AlgoThread::algoThread = this;

    nbSite = _nbSite;
    nbHabitants = _nbHabitants;
    nbBorne = _nbBorne;
    nbVelo = _nbVelo;

    connect(this,SIGNAL(initSite(int,int)),mainWindow,SLOT(initSite(int,int)));
    connect(this,SIGNAL(initHabitant(int,int)),mainWindow,SLOT(initHabitant(int,int)));

    connect(this,SIGNAL(setHabitantState(int,int)),mainWindow,SLOT(setHabitantState(int,int)));
    connect(this,SIGNAL(setSiteVelo(int,int)),mainWindow,SLOT(setSiteVelo(int,int)));
    connect(this,SIGNAL(startDeplacement(int,int,int,int)),mainWindow,SLOT(startDeplacement(int,int,int,int)));

    connect(this,SIGNAL(setDepotVelo(int)),mainWindow,SLOT(setDepotVelo(int)));
    connect(this,SIGNAL(setCamVelo(int)),mainWindow,SLOT(setCamVelo(int)));
    connect(this,SIGNAL(startCamionDeplacement(int,int,int)),mainWindow,SLOT(startCamionDeplacement(int,int,int)));
}
/**
 * @brief AlgoThread::~AlgoThread destructor
 */
AlgoThread::~AlgoThread()
{
    for(int i = 0; i < this->nbHabitants+1; i++)
    {
        delete this->arrThreads[i];
    }
    delete this->arrThreads;
    delete this->arrSites;
}

/**
 * @brief AlgoThread::threadSafeQDebug
 * function to display things in qDebug threadsafely
 * @param text
 */
void AlgoThread::threadSafeQDebug(QString text)
{
    if(this->displayQDebug)
    {
        this->mutexQDebug.lock();
        qDebug() << text;
        this->mutexQDebug.unlock();
    }
}

/**
 * @brief AlgoThread::addVelosAtSite
 * add nbVelos at a site, usually 1 or -1 velos
 * @param nbVelos
 * @param site
 */
void AlgoThread::addVelosAtSite(int nbVelos, Site *site)
{
    site->velosAtSite += nbVelos;
    emit this->setSiteVelo(site->id, site->velosAtSite);
}

/**
 * @brief AlgoThread::getAlgoThread
 * static function that allow to get the main thread from everywhere
 * @return
 */
AlgoThread *AlgoThread::getAlgoThread()
{
    return algoThread;
}

/**
 * @brief AlgoThread::getRandomTripTime
 * get a random trip time between one and waiting time
 * @return
 */
int AlgoThread::getRandomTripTime()
{
    return qrand() % waitingTime + 1;
}

/**
 * @brief AlgoThread::getRandomValue
 * get a random value between 0 and MAX
 * the seed can be parametred if necessary
 * @return random value between 0 and MAX
 */
int AlgoThread::getRandomValue(int max)
{
    return qrand() % max;
}

/**
 * @brief AlgoThread::setVeloPerSite
 * set X velos for each borne
 * @param velos
 */
void AlgoThread::setVelosPerSite(int velos)
{
    for(int i = 0; i < this->nbSite; i++)
    {
        emit setSiteVelo(i, velos);
        this->arrSites[i]->velosAtSite = velos;
    }
}
/**
 * @brief AlgoThread::setVelosAtDepot
 * setter for the number of velos at the depot
 * @param velos
 */
void AlgoThread::setVelosAtDepot(int velos)
{
    emit setDepotVelo(velos);
    this->nbVelosAtDepot = velos;
}

void AlgoThread::run()
{
    this->initSites();
    this->setVelosPerSite(this->nbBorne-2);
    this->setVelosAtDepot(this->nbVelo - (this->nbBorne-2)*this->nbSite);

    this->arrThreads = new QThread*[this->nbHabitants+1];
    this->createThreads();
    this->startThreads();
    this->waitThreads();
}

/**
 * @brief AlgoThread::createThreads
 * create all the threads and set them at their initial place
 */
void AlgoThread::createThreads()
{
    // create maintenance thread at the last position of the threads array
    this->arrThreads[this->nbHabitants] = new Maintenance();
    emit this->initCamion();

    // create habitant threads
    for(int i = 0; i < nbHabitants; i++)
    {
        int initSiteId = getRandomValue(nbSite);

        this->arrThreads[i] = new Habitant(i, initSiteId);
        emit this->initHabitant(i, initSiteId);
    }
}

/**
 * @brief AlgoThread::startThreads
 * start all the threads
 */
void AlgoThread::startThreads()
{
    for(int i = 0; i < nbHabitants+1; i++)
    {
        this->arrThreads[i]->start();
    }
}

/**
 * @brief AlgoThread::waitThreads
 * wait all the threads
 */
void AlgoThread::waitThreads()
{
    for(int i = 0; i < nbHabitants+1; i++)
    {
        this->arrThreads[i]->wait();
    }
}

/**
 * @brief AlgoThread::initSites
 * create all sites
 */
void AlgoThread::initSites()
{
    this->arrSites = new Site*[this->nbSite];

    for(int i = 0; i < this->nbSite; i++)
    {
        this->arrSites[i] = new Site(i);
    }
}
