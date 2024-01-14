# Patronum

<p align="center"><img src="res/Banner_web.jpg" alt="Credits"></p>

This is extension libraries for control your daemons and rervices. This library will save you the time you spend building and deploying server daemons or services. 

If you want to get more inforamtion about library see our [documentation](https://quasarapp.ddns.net:3031/docs/QuasarApp/Patronum/latest/)

### Why is Patronum?

Becouse This library offers easy interface to control your demons likewise the magic of Harry Potter controls dementors

## Main features

* Support linux systemd daemons.
* Deploy and install your services on the host.
* Auto create a Service from your server or yor daemon utility.
* Auto create a Controller of your Service. 

## Deffault sopprted commands

* install - deploys your daemon into your host system.  This command deploy service for root user if you want to deploy service for specify user just use   **-install <UserName>** command (root right required)
* unistall - removes old deployed daemon. (root right required)
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

    bool onStart() override {
        // call on server started
        return true;
    }

    void onStop() override {
        // call on server stoped 
    }

    bool handleReceive(const Patronum::Feature &data) override {
    
        if (data.cmd() == "ping") {
            sendResuylt("Pong");
        } else if (data.cmd() == "state") {
            sendResuylt("application status");
        } 

        return true;
    }


    QSet<Patronum::Feature> supportedFeatures() override {
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
Dy default the service can works as acontroller, But some times we need to create a simple controller app for exists our service. 
For example universal controller for all services of your comapny. The all services that based on Patronum library compatible with each other due to the fact that the command configuration is on the side of the service itself, and not the controller.


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
