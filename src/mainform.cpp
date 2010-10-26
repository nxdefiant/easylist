#include "mainform.h"
#include "ui_mainform.h"

MainForm::MainForm(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainForm)
{
    ui->setupUi(this);
    settings = new QSettings(WILLEM_LIU, EASY_LIST);

    newIndex = 0;
    connect(SystemSettings::getInstance(), SIGNAL(signalKeyboardClosed(bool)), this, SLOT(keyboardClosed(bool)));

    ui->actionAuto_Orientation->setChecked(settings->value(AUTO_ORIENTATION).toBool());
    on_actionAuto_Orientation_triggered();

    ui->actionSort_A_Z->setChecked(settings->value(SORT_A_Z).toBool());
    on_actionSort_A_Z_triggered();

    // Set a default value for CHECKED_ITEMS_TO_BOTTOM
    if(settings->contains(CHECKED_ITEMS_TO_BOTTOM) == false)
    {
        settings->setValue(CHECKED_ITEMS_TO_BOTTOM, false);
    }
    ui->actionChecked_bottom->setChecked(settings->value(CHECKED_ITEMS_TO_BOTTOM).toBool());
    on_actionChecked_bottom_triggered();

    // Create a default for landscape mode.
    landscape = settings->value(LANDSCAPE).toBool();
    // If LANDSCAPE exists in QSettings.
    if(settings->contains(LANDSCAPE))
    {
        // We use the LANDSCAPE value in the QSettings.
        landscape = settings->value(LANDSCAPE).toBool();
    }
    else
    {
        // Otherwise we set our default into the QSettings.
        settings->setValue(LANDSCAPE, landscape);
    }
    // If keyboard is opened at start. We do landscape mode.
    // Otherwise we do what's read from the QSettings.
    if(ui->actionAuto_Orientation->isChecked() == false)
    {
        if(SystemSettings::getInstance()->getKeyboardClosed() == false)
        {
            setLandscapeMode(true);
        }
        else
        {
            setLandscapeMode(landscape);
        }
    }

    // Populate the QStackedWidget. ListForm is set as the current widget.
    listForm = new ListForm(this);
    editForm = new EditForm(this);
    chooseListForm = new ChooseListForm(this);

    connect(listForm, SIGNAL(signalTransitionOutFinished()), this, SLOT(stateOutFinished()));
    connect(listForm, SIGNAL(signalNavigate(int)), this, SLOT(changeWidget(int)));

    connect(editForm, SIGNAL(signalTransitionOutFinished()), this, SLOT(stateOutFinished()));
    connect(editForm, SIGNAL(signalNavigate(int)), this, SLOT(changeWidget(int)));

    connect(chooseListForm, SIGNAL(signalTransitionOutFinished()), this, SLOT(stateOutFinished()));
    connect(chooseListForm, SIGNAL(signalNavigate(int)), this, SLOT(changeWidget(int)));

    ui->stackedWidget->addWidget(listForm);
    ui->stackedWidget->addWidget(editForm);
    ui->stackedWidget->addWidget(chooseListForm);
    ui->stackedWidget->setCurrentWidget(listForm);
}

MainForm::~MainForm()
{
    delete ui;
}

void MainForm::stateOutFinished()
{
    qDebug() << "Show new widget" << newIndex;
    ui->stackedWidget->setCurrentIndex(newIndex);
    SlideWidget * newWidget = dynamic_cast<SlideWidget * >(ui->stackedWidget->currentWidget());
    newWidget->move(0, -newWidget->height());
    newWidget->setStateIn();
    newWidget->shown();
}

void MainForm::changeWidget(int step)
{
    SlideWidget * currentWidget = dynamic_cast<SlideWidget * >(ui->stackedWidget->currentWidget());
    currentWidget->initStates();
    int currentIndex = ui->stackedWidget->indexOf(currentWidget);
    // Because all widgets are started with StateOut as initial state, we
    // need to reset the current widget to StateIn. The view is showing the
    // current widget at the place of its StateIn position. But the state
    // is never set to StateIn.
    currentWidget->setStateIn();
    qDebug() << "Current widget index" << currentIndex;
    newIndex = step;
    currentWidget->setStateOut();
    qDebug() << "New widget index" << newIndex;
}

void MainForm::keyboardClosed(bool closed)
{
    // When keyboard is opened.
    if(ui->actionAuto_Orientation->isChecked() == false)
    {
        if(closed == false)
        {
            setLandscapeMode(true);
        }
        else
        {
            setLandscapeMode(landscape);
        }
    }
}

void MainForm::setLandscapeMode(bool landscape)
{
    if(landscape)
    {
        tempLandscapeMode = true;
        qDebug() << LANDSCAPE;
#ifdef Q_WS_MAEMO_5
        setAttribute(Qt::WA_Maemo5AutoOrientation, false);
        setAttribute(Qt::WA_Maemo5LandscapeOrientation, true);
        setAttribute(Qt::WA_Maemo5PortraitOrientation, false);
#endif
    }
    else
    {
        tempLandscapeMode = false;
        qDebug() << PORTRAIT;
#ifdef Q_WS_MAEMO_5
        setAttribute(Qt::WA_Maemo5AutoOrientation, false);
        setAttribute(Qt::WA_Maemo5PortraitOrientation, true);
        setAttribute(Qt::WA_Maemo5LandscapeOrientation, false);
#endif
    }
}

void MainForm::on_actionRotate_triggered()
{
    qDebug() << "Rotate";

    landscape = (width() < height());
    settings->setValue(LANDSCAPE, landscape);
    ui->actionAuto_Orientation->setChecked(false);
    settings->setValue(AUTO_ORIENTATION, ui->actionAuto_Orientation->isChecked());
    setLandscapeMode(landscape);
}

void MainForm::on_actionAbout_triggered()
{
    qDebug() << "About";
    QString aboutText;
    aboutText.append("EasyList (c) 2010\n\n");
    aboutText.append("Created by Willem Liu.\n");
    aboutText.append("Created with QtCreator.\n");
    QMessageBox::about(this, "EasyList", aboutText);
}

void MainForm::on_actionChecked_bottom_triggered()
{
    bool sortToBottom = ui->actionChecked_bottom->isChecked();
    qDebug() << "Checked Bottom" << sortToBottom;
    settings->setValue(CHECKED_ITEMS_TO_BOTTOM, sortToBottom);
    MyCheckBoxContainer::getInstance()->setSortCheckedToBottom(sortToBottom);
}

void MainForm::closeEvent(QCloseEvent *event)
{
    settings->setValue(LIST_TEXT, MyCheckBoxContainer::getInstance()->getListText());
    event->accept();
}

void MainForm::on_actionAuto_Orientation_triggered()
{
    settings->setValue(AUTO_ORIENTATION, ui->actionAuto_Orientation->isChecked());
    qDebug() << "Auto orientation" << ui->actionAuto_Orientation->isChecked();
    if(ui->actionAuto_Orientation->isChecked())
    {
#ifdef Q_WS_MAEMO_5
        setAttribute(Qt::WA_Maemo5PortraitOrientation, false);
        setAttribute(Qt::WA_Maemo5LandscapeOrientation, false);
        setAttribute(Qt::WA_Maemo5AutoOrientation, true);
#endif
    }
    else
    {
        setLandscapeMode(landscape);
    }
}

void MainForm::on_actionSort_A_Z_triggered()
{
    settings->setValue(SORT_A_Z, ui->actionSort_A_Z->isChecked());
    MyCheckBoxContainer::getInstance()->setSortAlphabetically(ui->actionSort_A_Z->isChecked());
}

void MainForm::on_actionLists_triggered()
{
    listForm->saveList();
    changeWidget(2);
}
