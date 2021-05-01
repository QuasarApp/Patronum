# Patronum

This is extension libraries for control your daemons and rervices. This library will save you the time you spend building and deploying server daemons or services.

### Why is Patronum?

Becouse This library offers easy interface to control your demons likewise the magic of Harry Potter controls dementors

## Main features

* Support linux systemd daemons.
* Deploy and install your services on the host.
* Auto create a Service from your server or yor daemon utility.
* Auto create a Controller of your Service. 

## Deffault sopprted commands
* install - deploys your daemon into your host system.
* unistall - removes old deployed daemon.
* start - starts your service
* stop - stops ypur service
* pause - sends pause command to your daemon
* upause (resume) - sends resume coomand to your daemon


## Include

### For cmake projects
 
 * cd yourRepo
 * git submodule add https://github.com/QuasarApp/Patronum.git # add the repository of Patronum into your repo like submodule
 * git submodule update --init --recursive
 * Include in your CMakeLists.txt file the main CMakeLists.txt file of Patronum library
  ```cmake
    add_subdirectory(Patronum)
  ```
 * Rebuild yuor project


## Usage

### Service

```cpp
#include <patronum.h>

class MyserviceApp : public Patronum::Service<QCoreApplication>
{

public:
    MyserviceApp(int argc, char **argv):
        Patronum::Service<QCoreApplication>(argc, argv) {

    }

    void start() {
        // call on server started 
    }

    void stop() {
        // call on server stoped 
    }

    bool HanoiService::handleReceive(const Patronum::Feature &data) {
    
        if (data.cmd() == "ping") {
            sendResuylt("Pong");
        } else if (data.cmd() == "state") {
            sendResuylt("application status");
        } 

        return true;
    }


    QList<Feature> supportedFeatures() {
        QSet<Patronum::Feature> data;

        data << Patronum::Feature("ping", "This is description of the ping command");
        data << Patronum::Feature("state", "return state");

        return data;
    }
};


int main(int argc, char **argv) {
    QCoreApplication::setApplicationName("MyServiceName"); // <--
    QCoreApplication::setOrganizationName("MyCompany"); // <--
    MyserviceApp app;
    return app.exec();
}
```

### Controller

```cpp
#include <patronum.h>

class MyControllerApp : public Patronum::Controller
{
public:

    MyControllerApp():
        Patronum::Controller() {

    }
};

int main(int argc, char **argv) {
    QCoreApplication::setApplicationName("MyServiceName"); // <--
    QCoreApplication::setOrganizationName("MyCompany"); // <--
    QCoreApplcication app
    MyControllerApp controller;
    controller.send(argc, argv);
    return app.exec();
}
```


### Wrapers of the service distributions

The Service class use own executable like a main path to service executable. 
If your application has custom dependencies and do not work without costom envirement then you need to add **P_RUN_FILE** or **CQT_RUN_FILE** variavle with full path into the your wraper or launcher file.

#### Order of the search executable file of the service

1. **P_RUN_FILE** variable
2. **CQT_RUN_FILE** variable
3. Absalute path to executable.
