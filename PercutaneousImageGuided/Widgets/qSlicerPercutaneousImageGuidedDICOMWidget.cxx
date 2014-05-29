/*==============================================================================

  Program: 3D Slicer

  Copyright (c) Kitware Inc.

  See COPYRIGHT.txt
  or http://www.slicer.org/copyright/copyright.txt for details.

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.

  This file was originally developed by Jean-Christophe Fillion-Robin, Kitware Inc.
  and was partially funded by NIH grant 3P41RR013218-12S1

==============================================================================*/

// DICOM Widgets includes
#include "qSlicerPercutaneousImageGuidedDICOMWidget.h"
#include "ui_qSlicerPercutaneousImageGuidedDICOMWidget.h"

#include "vtkSlicerConfigure.h"

#include "vtkMRMLPercutaneousImageGuidedParameterNode.h"

#include "ctkDICOMBrowser.h"
#include "ctkDICOMDatabase.h"

//-----------------------------------------------------------------------------
/// \ingroup Slicer_QtModules_PercutaneousImageGuided
class qSlicerPercutaneousImageGuidedDICOMWidgetPrivate
  : public Ui_qSlicerPercutaneousImageGuidedDICOMWidget
{
  Q_DECLARE_PUBLIC(qSlicerPercutaneousImageGuidedDICOMWidget);
protected:
  qSlicerPercutaneousImageGuidedDICOMWidget* const q_ptr;

public:
  qSlicerPercutaneousImageGuidedDICOMWidgetPrivate(
    qSlicerPercutaneousImageGuidedDICOMWidget& object);
  ~qSlicerPercutaneousImageGuidedDICOMWidgetPrivate();
  virtual void setupUi(qSlicerPercutaneousImageGuidedDICOMWidget*);

  int startStoreSCP(QString path, QString aetitle, QString port);
  void stopStoreSCP();
  void showDICOMBrowser();
  void hideDICOMBrowser();

private:
  vtkMRMLPercutaneousImageGuidedParameterNode* activeParamNode;

  QProcess* storeSCP;
  ctkDICOMBrowser* dicomBrowser;
};

// --------------------------------------------------------------------------
qSlicerPercutaneousImageGuidedDICOMWidgetPrivate
::qSlicerPercutaneousImageGuidedDICOMWidgetPrivate(
  qSlicerPercutaneousImageGuidedDICOMWidget& object)
  : q_ptr(&object)
{
  this->storeSCP = new QProcess();
  this->dicomBrowser = new ctkDICOMBrowser();
}

// --------------------------------------------------------------------------
qSlicerPercutaneousImageGuidedDICOMWidgetPrivate
::~qSlicerPercutaneousImageGuidedDICOMWidgetPrivate()
{
  if (this->storeSCP)
    {
    this->stopStoreSCP();
    this->storeSCP->deleteLater();
    }

  if (this->dicomBrowser)
    {
    this->dicomBrowser->database()->closeDatabase();
    this->dicomBrowser->deleteLater();
    }
}

// --------------------------------------------------------------------------
void qSlicerPercutaneousImageGuidedDICOMWidgetPrivate
::setupUi(qSlicerPercutaneousImageGuidedDICOMWidget* widget)
{
  this->Ui_qSlicerPercutaneousImageGuidedDICOMWidget::setupUi(widget);
}

//-----------------------------------------------------------------------------
int qSlicerPercutaneousImageGuidedDICOMWidgetPrivate::startStoreSCP(QString path, QString aetitle, QString port)
{
  if (path.isEmpty() || aetitle.isEmpty() || port.isEmpty())
    {
    return 0;
    }

  // Stop storeSCP if already running
  this->stopStoreSCP();

  // Find storescp (DCMTK)
  std::stringstream storescpBin;
  storescpBin << VTK_DIR << "/../DCMTK-build/bin/storescp";

  // Create parameters
  QStringList storescpParameters;
  storescpParameters << "--output-directory" << path << "-aet" << aetitle << port;

  // Start process
  this->storeSCP->start(storescpBin.str().c_str(), storescpParameters);
  this->storeSCP->waitForStarted(5000);
  
  if (this->storeSCP->state() == QProcess::NotRunning)
    {
    return 0;
    }

  return 1;
}

//-----------------------------------------------------------------------------
void qSlicerPercutaneousImageGuidedDICOMWidgetPrivate::stopStoreSCP()
{
  if (this->storeSCP->state() == QProcess::NotRunning)
    {
    return;
    }
  
  // Try to terminate process properly
  this->storeSCP->terminate();
  this->storeSCP->waitForFinished(5000);
  if (this->storeSCP->state() == QProcess::Running)
    {
    // if process doesn't stop after 5s, kill it
    this->storeSCP->kill();
    }
}

//-----------------------------------------------------------------------------
void qSlicerPercutaneousImageGuidedDICOMWidgetPrivate::showDICOMBrowser()
{
  if (this->dicomBrowser)
    {
    this->dicomBrowser->show();
    }
}

//-----------------------------------------------------------------------------
void qSlicerPercutaneousImageGuidedDICOMWidgetPrivate::hideDICOMBrowser()
{
  if (this->dicomBrowser)
    {
    this->dicomBrowser->hide();
    }
}

//-----------------------------------------------------------------------------
// qSlicerPercutaneousImageGuidedDICOMWidget methods

//-----------------------------------------------------------------------------
qSlicerPercutaneousImageGuidedDICOMWidget
::qSlicerPercutaneousImageGuidedDICOMWidget(QWidget* parentWidget)
  : Superclass( parentWidget )
  , d_ptr( new qSlicerPercutaneousImageGuidedDICOMWidgetPrivate(*this) )
{
  Q_D(qSlicerPercutaneousImageGuidedDICOMWidget);
  d->setupUi(this);

  connect(d->DICOMListenerButton, SIGNAL(directoryChanged(const QString&)),
	  this, SLOT(onDICOMDirectoryChanged(const QString&)));

  connect(d->DICOMManagerButton, SIGNAL(clicked()),
	  this, SLOT(onDICOMManagerClicked()));
}

//-----------------------------------------------------------------------------
qSlicerPercutaneousImageGuidedDICOMWidget
::~qSlicerPercutaneousImageGuidedDICOMWidget()
{
  this->qvtkDisconnectAll();
}

// --------------------------------------------------------------------------
void qSlicerPercutaneousImageGuidedDICOMWidget
::setActiveParameterNode(vtkMRMLPercutaneousImageGuidedParameterNode* activeNode)
{
  Q_D(qSlicerPercutaneousImageGuidedDICOMWidget);

  vtkMRMLPercutaneousImageGuidedParameterNode* currentActiveNode = d->activeParamNode;
  if (currentActiveNode)
    {
    this->qvtkDisconnect(currentActiveNode, vtkMRMLPercutaneousImageGuidedParameterNode::DICOMModifiedEvent,
			 this, SLOT(refreshWidget()));
    }

  d->activeParamNode = activeNode;
  this->refreshWidget();

  this->qvtkConnect(d->activeParamNode, vtkMRMLPercutaneousImageGuidedParameterNode::DICOMModifiedEvent,
		    this, SLOT(refreshWidget()));
}

// --------------------------------------------------------------------------
vtkMRMLPercutaneousImageGuidedParameterNode* qSlicerPercutaneousImageGuidedDICOMWidget
::getActiveParameterNode()
{
  Q_D(qSlicerPercutaneousImageGuidedDICOMWidget);

  return d->activeParamNode;
}

//-----------------------------------------------------------------------------
void qSlicerPercutaneousImageGuidedDICOMWidget::onDICOMDirectoryChanged(const QString& directory)
{
  Q_D(qSlicerPercutaneousImageGuidedDICOMWidget);

  QString port = d->DICOMPortBox->text();
  QString aetitle = d->DICOMAETitleBox->text();

  if (d->startStoreSCP(directory, aetitle, port) != 0)
    {
    d->DICOMDirectoryLabel->setText(directory);
    }
}

//-----------------------------------------------------------------------------
void qSlicerPercutaneousImageGuidedDICOMWidget::onDICOMManagerClicked()
{   
  Q_D(qSlicerPercutaneousImageGuidedDICOMWidget);
  
  d->showDICOMBrowser();
}

//-----------------------------------------------------------------------------
void qSlicerPercutaneousImageGuidedDICOMWidget::refreshWidget()
{
  std::cerr << "Udpate DICOM Widget" << std::endl;
}
