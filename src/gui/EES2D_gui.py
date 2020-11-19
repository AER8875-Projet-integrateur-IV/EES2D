'''/* ---------------------------------------------------------------------
 *
 * Copyright (C) 2020 - by the EES2D authors
 *
 * This file is part of EES2D.
 *
 *   EES2D is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   EES2D is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with EES2D.  If not, see <https://www.gnu.org/licenses/>.
 *
 * ---------------------------------------------------------------------
 *
 * Authors: Amin Ouled-Mohamed & Ali Omais, Polytechnique Montreal, 2020-
 */'''
from PySide2.QtCore import *
from PySide2.QtGui import *
from PySide2.QtWidgets import *
import sys
from datetime import datetime
import os
import subprocess

class Ui_MainWindow(object):
    
    def GenerateSolverFile(self):
    # Get user input
        self.speedType = self.comboBoxSpeedType.currentText()
        self.speedvalue = self.VelocityEdit.toPlainText()
        self.aoa = self.AOAedit.toPlainText()
        self.pressure = self.PressureEdit.toPlainText()
        self.Temperature = self.TemperatureEdit.toPlainText()
        self.Viscosity = self.ViscosityEdit.toPlainText()
        self.Density = self.DensityEdit.toPlainText()
        self.Gamma = self.GammaEdit.toPlainText()
        self.GasConstant = self.GasConstantEdit.toPlainText()
        self.SpecificHeat = self.SpecificHeatEdit.toPlainText()
        self.TimeIntegration = self.TimeIntegrationComboBox.currentText()
        self.CflValue = self.CflEdit.toPlainText()
        self.MaxResidual = self.minResidualEdit.toPlainText()
        self.MaxIter = self.MaxIterEdit.toPlainText()
        self.NumThreads = self.NumThreadsEdit.toPlainText()
        if self.speedType == "Mach":
            self.speedRightType = "MACH"
            self.VelocityValue = 0
            self.MachValue = self.speedvalue
        elif self.speedType == "Velocity (m/s)":
            self.speedRightType = "VELOCITY"
            self.VelocityValue = self.speedvalue
            self.MachValue = 0
        
        if self.TimeIntegration == "Runge-Kutta 5":
            self.RightTimeIntegration = "RK5"
        elif self.TimeIntegration == "Explicit Euler":
            self.RightTimeIntegration = "EXPLICIT_EULER"
        


    # Generate Control File
        self.ControlFileName =  "ControlFile-" + datetime.now().strftime('%Y-%m-%d--%H-%M-%S')
        filestream = open(self.ControlFileName + ".ees2d", "w")
    #Writing to Control File
        filestream.write(f"-------------------------------------------------------------------------------\n" \
                "%%%%%%%%%%%%%%%%     EES2D Software Input file    %%%%%%%%%%%%%%%%%%%%% \n" \
                "Author : EES2D GUI\n" \
                "Simulation Title : \n" \
                f"Date : {datetime.now().strftime('%Y-%m-%d %H:%M:%S')}\n" \
                "-------------------------------------------------------------------------------\n" \
                "\nSTART\n\n" \
                "------------------- PRE-PROCESSING CONTROL -------------------\n\n" \
                "MESH_FORMAT = SU2\n\n" \
                f"MESH_FILE = {self.Meshpath}\n\n" \
                "MESH_TYPE = UNSTRUCTURED\n\n" \
                "------------------- SIMULATION CONTROL -------------------\n\n" \
                f"SPEED_OPTION = {self.speedRightType}\n\n" \
                f"VELOCITY = {self.VelocityValue}\n\n" \
                f"MACH = {self.MachValue}\n\n" \
                f"AOA = {self.aoa}\n\n" \
                f"AIRFLOW_PRESSURE = {self.pressure}\n\n" \
                f"AIRFLOW_TEMPERATURE = {self.Temperature}\n\n" \
                f"AIRFLOW_VISCOSITY = {self.Viscosity}\n\n" \
                f"AIRFLOW_DENSITY = {self.Density}\n\n" \
                f"GAMMA = {self.Gamma}\n\n" \
                f"GAS_CONSTANT = {self.GasConstant}\n\n" \
                f"SPECIFIC_HEAT = {self.SpecificHeat}\n\n" \
                "------------------- SOLVER CONTROL -------------------\n\n" \
                "SCHEME = ROE\n\n" \
                f"TIME_INTEGRATION = {self.RightTimeIntegration}\n\n" \
                f"CFL = {self.CflValue}\n\n" \
                f"MIN_RESIDUAL = {self.MaxResidual}\n\n" \
                f"MAX_ITER = {self.MaxIter}\n\n" \
                f"OPENMP_THREADS_NUM = {self.NumThreads}\n\n" \
                "------------------- POST-PROCESSING CONTROL -------------------\n\n" \
                f"RESIDUAL_FILE = {self.ResidualPath}\n\n" \
                f"PRESSURE_FILE = {self.PressurePath}\n\n" \
                "OUTPUT_FORMAT = VTK\n\n" \
                f"OUTPUT_FILE = {self.VtuPath}\n\n" \
                f"GENERATE_LOG = YES\n\n" \
                "END\n")
        filestream.close()
        self.GeneratedOutputFileMessage()

    def RunSolverFunc(self):
        self.GenerateSolverFile()
        os.system(f"EES2D_App.exe {self.ControlFileName}.ees2d")
        
    
    def GeneratedOutputFileMessage(self):
        Dialog = QDialog()
        if not Dialog.objectName():
            Dialog.setObjectName(u"Dialog")
        Dialog.resize(400, 250)
        Dialog.setMinimumSize(QSize(400, 250))
        Dialog.setMaximumSize(QSize(400, 250))
        Dialog.setWindowIcon(QIcon('Logo/logo.png'))
        self.ControlFileMsgLabel = QLabel(Dialog)
        self.ControlFileMsgLabel.setObjectName(u"ControlFileMsgLabel")
        self.ControlFileMsgLabel.setGeometry(QRect(80, 30, 251, 71))
        font = QFont()
        font.setPointSize(11)
        self.ControlFileMsgLabel.setFont(font)
        self.ConfirmControlFileMsgButton = QPushButton(Dialog)
        self.ConfirmControlFileMsgButton.setObjectName(u"ConfirmControlFileMsgButton")
        self.ConfirmControlFileMsgButton.setGeometry(QRect(150, 190, 93, 28))
        self.ConfirmControlFileMsgButton.clicked.connect(Dialog.accept)
        self.ControlFileNameMsgLabel = QLabel(Dialog)
        self.ControlFileNameMsgLabel.setObjectName(u"ControlFileNameMsgLabel")
        self.ControlFileNameMsgLabel.setGeometry(QRect(30, 120, 341, 31))
        self.ControlFileMsgLabel.setAlignment(Qt.AlignCenter)
        
        self.retranslateUi(Dialog)

        QMetaObject.connectSlotsByName(Dialog)
        Dialog.setWindowTitle(QCoreApplication.translate("EES2D Alert", u"EES2D Alert", None))
        self.ControlFileMsgLabel.setText(QCoreApplication.translate("Dialog", u"EES2D control file generated !", None))
        self.ConfirmControlFileMsgButton.setText(QCoreApplication.translate("Dialog", u"Ok", None))
        self.ControlFileNameMsgLabel.setText(QCoreApplication.translate("Dialog", f"Control file name : {self.ControlFileName}.ees2d ", None))
        Dialog.exec_()
    # setupUi

    
    def GetMeshPath(self):
        dlg = QFileDialog()
        dlg.setFileMode(QFileDialog.AnyFile)
        dlg.setNameFilters(['*.su2'])
        dlg.setAcceptMode(QFileDialog.AcceptOpen)
        if dlg.exec_() == QDialog.Accepted:
            self.Meshpath = dlg.selectedFiles()[0]  # returns a list
            self.MeshPathBrowser.setText(dlg.selectedFiles()[0])
    
    def GetPressurePath(self):
        dlg = QFileDialog()
        dlg.setFileMode(QFileDialog.AnyFile)
        dlg.setNameFilters(['*.dat'])
        dlg.setAcceptMode(QFileDialog.AcceptSave)
        if dlg.exec_() == QDialog.Accepted:
            self.PressurePath = dlg.selectedFiles()[0]  # returns a list
            self.PressurePathBrowser.setText(dlg.selectedFiles()[0])

    def GetResidualPath(self):
        dlg = QFileDialog()
        dlg.setFileMode(QFileDialog.AnyFile)
        dlg.setNameFilters(['*.dat'])
        dlg.setAcceptMode(QFileDialog.AcceptSave)
        if dlg.exec_() == QDialog.Accepted:
            self.ResidualPath = dlg.selectedFiles()[0]  # returns a list
            self.ResidualPathBrowser.setText(dlg.selectedFiles()[0])

    def GetVtuPath(self):
        dlg = QFileDialog()
        dlg.setFileMode(QFileDialog.AnyFile)
        dlg.setNameFilters(['*.vtu'])
        dlg.setAcceptMode(QFileDialog.AcceptSave)
        if dlg.exec_() == QDialog.Accepted:
            self.VtuPath = dlg.selectedFiles()[0]  # returns a list
            self.OutputPathBrowser.setText(dlg.selectedFiles()[0])
			
        
    def setupUi(self, MainWindow):
        
            
        if not MainWindow.objectName():
            MainWindow.setObjectName(u"MainWindow")
        MainWindow.resize(600, 680)
        MainWindow.setMinimumSize(QSize(600, 650))
        MainWindow.setMaximumSize(QSize(600, 680))
        MainWindow.setIconSize(QSize(30, 30))
        self.centralwidget = QWidget(MainWindow)
        self.centralwidget.setObjectName(u"centralwidget")
        self.GenerateControl = QPushButton(self.centralwidget)
        self.GenerateControl.setObjectName(u"GenerateControl")
        self.GenerateControl.setGeometry(QRect(320, 580, 191, 41))
        self.GenerateControl.clicked.connect(self.GenerateSolverFile)
        
        self.RunSolver = QPushButton(self.centralwidget)
        self.RunSolver.setObjectName(u"RunSolver")
        self.RunSolver.setGeometry(QRect(80, 580, 171, 41))
        self.RunSolver.clicked.connect(self.RunSolverFunc)
        
        self.tabWidget = QTabWidget(self.centralwidget)
        self.tabWidget.setObjectName(u"tabWidget")
        self.tabWidget.setGeometry(QRect(10, 10, 581, 561))
        self.tabWidget.setAutoFillBackground(True)
        self.PreTab = QWidget()
        self.PreTab.setObjectName(u"PreTab")
        self.MeshLabel = QLabel(self.PreTab)
        self.MeshLabel.setObjectName(u"MeshLabel")
        self.MeshLabel.setGeometry(QRect(30, 60, 101, 71))
        font = QFont()
        font.setPointSize(10)
        self.MeshLabel.setFont(font)
        self.MeshPathBrowser = QTextBrowser(self.PreTab)
        self.MeshPathBrowser.setObjectName(u"MeshPathBrowser")
        self.MeshPathBrowser.setGeometry(QRect(150, 80, 291, 41))
        self.MeshFileFinder = QToolButton(self.PreTab)
        self.MeshFileFinder.setObjectName(u"MeshFileFinder")
        self.MeshFileFinder.setGeometry(QRect(450, 80, 41, 31))
        self.MeshFileFinder.clicked.connect(self.GetMeshPath)
        self.tabWidget.addTab(self.PreTab, "")
        self.simTab = QWidget()
        self.simTab.setObjectName(u"simTab")
        self.comboBoxSpeedType = QComboBox(self.simTab)
        self.comboBoxSpeedType.addItem("")
        self.comboBoxSpeedType.addItem("")
        self.comboBoxSpeedType.setObjectName(u"comboBoxSpeedType")
        self.comboBoxSpeedType.setGeometry(QRect(240, 20, 111, 31))
        self.SpeedTypeLabel = QLabel(self.simTab)
        self.SpeedTypeLabel.setObjectName(u"SpeedTypeLabel")
        self.SpeedTypeLabel.setGeometry(QRect(110, 20, 111, 31))
        self.SpeedTypeLabel.setFont(font)
        self.VelocityValueLabel = QLabel(self.simTab)
        self.VelocityValueLabel.setObjectName(u"VelocityValueLabel")
        self.VelocityValueLabel.setGeometry(QRect(100, 70, 111, 21))
        self.VelocityValueLabel.setFont(font)
        self.aoaLabel = QLabel(self.simTab)
        self.aoaLabel.setObjectName(u"aoaLabel")
        self.aoaLabel.setGeometry(QRect(90, 120, 121, 31))
        self.aoaLabel.setFont(font)
        self.PressureLabel = QLabel(self.simTab)
        self.PressureLabel.setObjectName(u"PressureLabel")
        self.PressureLabel.setGeometry(QRect(140, 170, 71, 31))
        self.PressureLabel.setFont(font)
        self.TempLabel = QLabel(self.simTab)
        self.TempLabel.setObjectName(u"TempLabel")
        self.TempLabel.setGeometry(QRect(110, 220, 101, 31))
        self.TempLabel.setFont(font)
        self.ViscosityLabel = QLabel(self.simTab)
        self.ViscosityLabel.setObjectName(u"ViscosityLabel")
        self.ViscosityLabel.setGeometry(QRect(140, 270, 81, 31))
        self.ViscosityLabel.setFont(font)
        self.DensityLabel = QLabel(self.simTab)
        self.DensityLabel.setObjectName(u"DensityLabel")
        self.DensityLabel.setGeometry(QRect(150, 320, 61, 31))
        self.DensityLabel.setFont(font)
        self.GammaLabel = QLabel(self.simTab)
        self.GammaLabel.setObjectName(u"GammaLabel")
        self.GammaLabel.setGeometry(QRect(150, 370, 61, 31))
        self.GammaLabel.setFont(font)
        self.GasCstLabel = QLabel(self.simTab)
        self.GasCstLabel.setObjectName(u"GasCstLabel")
        self.GasCstLabel.setGeometry(QRect(110, 420, 101, 31))
        self.GasCstLabel.setFont(font)
        self.SpecificHeatLabel = QLabel(self.simTab)
        self.SpecificHeatLabel.setObjectName(u"SpecificHeatLabel")
        self.SpecificHeatLabel.setGeometry(QRect(110, 470, 101, 31))
        self.SpecificHeatLabel.setFont(font)
        self.VelocityEdit = QTextEdit(self.simTab)
        self.VelocityEdit.setObjectName(u"VelocityEdit")
        self.VelocityEdit.setGeometry(QRect(240, 70, 111, 31))
        self.VelocityEdit.setTabChangesFocus(True)
        self.AOAedit = QTextEdit(self.simTab)
        self.AOAedit.setObjectName(u"AOAedit")
        self.AOAedit.setGeometry(QRect(240, 120, 111, 31))
        self.AOAedit.setTabChangesFocus(True)
        self.PressureEdit = QTextEdit(self.simTab)
        self.PressureEdit.setObjectName(u"PressureEdit")
        self.PressureEdit.setGeometry(QRect(240, 170, 111, 31))
        self.PressureEdit.setTabChangesFocus(True)
        self.TemperatureEdit = QTextEdit(self.simTab)
        self.TemperatureEdit.setObjectName(u"TemperatureEdit")
        self.TemperatureEdit.setGeometry(QRect(240, 220, 111, 31))
        self.TemperatureEdit.setTabChangesFocus(True)
        self.ViscosityEdit = QTextEdit(self.simTab)
        self.ViscosityEdit.setObjectName(u"ViscosityEdit")
        self.ViscosityEdit.setGeometry(QRect(240, 270, 111, 31))
        self.ViscosityEdit.setTabChangesFocus(True)
        self.DensityEdit = QTextEdit(self.simTab)
        self.DensityEdit.setObjectName(u"DensityEdit")
        self.DensityEdit.setGeometry(QRect(240, 320, 111, 31))
        self.DensityEdit.setTabChangesFocus(True)
        self.GammaEdit = QTextEdit(self.simTab)
        self.GammaEdit.setObjectName(u"GammaEdit")
        self.GammaEdit.setGeometry(QRect(240, 370, 111, 31))
        self.GammaEdit.setTabChangesFocus(True)
        self.GasConstantEdit = QTextEdit(self.simTab)
        self.GasConstantEdit.setObjectName(u"GasConstantEdit")
        self.GasConstantEdit.setGeometry(QRect(240, 420, 111, 31))
        self.GasConstantEdit.setTabChangesFocus(True)
        self.SpecificHeatEdit = QTextEdit(self.simTab)
        self.SpecificHeatEdit.setObjectName(u"SpecificHeatEdit")
        self.SpecificHeatEdit.setGeometry(QRect(240, 470, 111, 31))
        self.SpecificHeatEdit.setTabChangesFocus(True)
        self.AOAunits = QLabel(self.simTab)
        self.AOAunits.setObjectName(u"AOAunits")
        self.AOAunits.setGeometry(QRect(360, 120, 71, 31))
        self.pressureUnits = QLabel(self.simTab)
        self.pressureUnits.setObjectName(u"pressureUnits")
        self.pressureUnits.setGeometry(QRect(360, 170, 71, 31))
        self.TempUnits = QLabel(self.simTab)
        self.TempUnits.setObjectName(u"TempUnits")
        self.TempUnits.setGeometry(QRect(360, 220, 71, 31))
        self.ViscosityUnits = QLabel(self.simTab)
        self.ViscosityUnits.setObjectName(u"ViscosityUnits")
        self.ViscosityUnits.setGeometry(QRect(360, 270, 71, 31))
        self.DensityUnits = QLabel(self.simTab)
        self.DensityUnits.setObjectName(u"DensityUnits")
        self.DensityUnits.setGeometry(QRect(360, 320, 71, 31))
        self.GasConstantUnits = QLabel(self.simTab)
        self.GasConstantUnits.setObjectName(u"GasConstantUnits")
        self.GasConstantUnits.setGeometry(QRect(360, 420, 71, 31))
        self.SpecificHeatUnits = QLabel(self.simTab)
        self.SpecificHeatUnits.setObjectName(u"SpecificHeatUnits")
        self.SpecificHeatUnits.setGeometry(QRect(360, 470, 71, 31))
        self.tabWidget.addTab(self.simTab, "")
        self.SolverTab = QWidget()
        self.SolverTab.setObjectName(u"SolverTab")
        self.TimeIntegrationLabel = QLabel(self.SolverTab)
        self.TimeIntegrationLabel.setObjectName(u"TimeIntegrationLabel")
        self.TimeIntegrationLabel.setGeometry(QRect(60, 30, 151, 31))
        self.TimeIntegrationLabel.setFont(font)
        self.CflLabel = QLabel(self.SolverTab)
        self.CflLabel.setObjectName(u"CflLabel")
        self.CflLabel.setGeometry(QRect(150, 120, 51, 31))
        self.CflLabel.setFont(font)
        self.MinResidualLabel = QLabel(self.SolverTab)
        self.MinResidualLabel.setObjectName(u"MinResidualLabel")
        self.MinResidualLabel.setGeometry(QRect(50, 210, 131, 31))
        self.MinResidualLabel.setFont(font)
        self.MaxIterLabel = QLabel(self.SolverTab)
        self.MaxIterLabel.setObjectName(u"MaxIterLabel")
        self.MaxIterLabel.setGeometry(QRect(10, 300, 171, 31))
        self.MaxIterLabel.setFont(font)
        self.numThreadsLabel = QLabel(self.SolverTab)
        self.numThreadsLabel.setObjectName(u"numThreadsLabel")
        self.numThreadsLabel.setGeometry(QRect(30, 390, 151, 31))
        self.numThreadsLabel.setFont(font)
        self.TimeIntegrationComboBox = QComboBox(self.SolverTab)
        self.TimeIntegrationComboBox.addItem("")
        self.TimeIntegrationComboBox.addItem("")
        self.TimeIntegrationComboBox.setObjectName(u"TimeIntegrationComboBox")
        self.TimeIntegrationComboBox.setGeometry(QRect(240, 30, 121, 31))
        self.CflEdit = QTextEdit(self.SolverTab)
        self.CflEdit.setObjectName(u"CflEdit")
        self.CflEdit.setGeometry(QRect(240, 120, 121, 31))
        self.CflEdit.setTabChangesFocus(True)
        
        self.minResidualEdit = QTextEdit(self.SolverTab)
        self.minResidualEdit.setObjectName(u"minResidualEdit")
        self.minResidualEdit.setGeometry(QRect(240, 210, 121, 31))
        self.minResidualEdit.setTabChangesFocus(True)
        self.MaxIterEdit = QTextEdit(self.SolverTab)
        self.MaxIterEdit.setObjectName(u"MaxIterEdit")
        self.MaxIterEdit.setGeometry(QRect(240, 300, 121, 31))
        self.MaxIterEdit.setTabChangesFocus(True)
        self.NumThreadsEdit = QTextEdit(self.SolverTab)
        self.NumThreadsEdit.setObjectName(u"NumThreadsEdit")
        self.NumThreadsEdit.setGeometry(QRect(240, 390, 121, 31))
        self.NumThreadsEdit.setTabChangesFocus(True)
        self.tabWidget.addTab(self.SolverTab, "")
        self.tab = QWidget()
        self.tab.setObjectName(u"tab")
        self.PressureFileLabel = QLabel(self.tab)
        self.PressureFileLabel.setObjectName(u"PressureFileLabel")
        self.PressureFileLabel.setGeometry(QRect(60, 40, 100, 70))
        self.PressureFileLabel.setFont(font)
        self.ResidualFileLabel = QLabel(self.tab)
        self.ResidualFileLabel.setObjectName(u"ResidualFileLabel")
        self.ResidualFileLabel.setGeometry(QRect(60, 150, 100, 70))
        self.ResidualFileLabel.setFont(font)
        self.OutputFileLabel = QLabel(self.tab)
        self.OutputFileLabel.setObjectName(u"OutputFileLabel")
        self.OutputFileLabel.setGeometry(QRect(40, 260, 130, 70))
        self.OutputFileLabel.setFont(font)
        self.PressurePathBrowser = QTextBrowser(self.tab)
        self.PressurePathBrowser.setObjectName(u"PressurePathBrowser")
        self.PressurePathBrowser.setGeometry(QRect(180, 60, 291, 41))
        self.ResidualPathBrowser = QTextBrowser(self.tab)
        self.ResidualPathBrowser.setObjectName(u"ResidualPathBrowser")
        self.ResidualPathBrowser.setGeometry(QRect(180, 170, 291, 41))
        self.OutputPathBrowser = QTextBrowser(self.tab)
        self.OutputPathBrowser.setObjectName(u"OutputPathBrowser")
        self.OutputPathBrowser.setGeometry(QRect(180, 280, 291, 41))
        self.PressureFileFinder = QToolButton(self.tab)
        self.PressureFileFinder.setObjectName(u"PressureFileFinder")
        self.PressureFileFinder.setGeometry(QRect(480, 60, 41, 31))
        self.PressureFileFinder.clicked.connect(self.GetPressurePath)

        self.ResidualFileFinder = QToolButton(self.tab)
        self.ResidualFileFinder.setObjectName(u"ResidualFileFinder")
        self.ResidualFileFinder.setGeometry(QRect(480, 170, 41, 31))
        self.ResidualFileFinder.clicked.connect(self.GetResidualPath)

        self.OutputFileFinder = QToolButton(self.tab)
        self.OutputFileFinder.setObjectName(u"OutputFileFinder")
        self.OutputFileFinder.setGeometry(QRect(480, 280, 41, 31))
        self.OutputFileFinder.clicked.connect(self.GetVtuPath)

        self.tabWidget.addTab(self.tab, "")
        MainWindow.setCentralWidget(self.centralwidget)
        self.menubar = QMenuBar(MainWindow)
        self.menubar.setObjectName(u"menubar")
        self.menubar.setGeometry(QRect(0, 0, 600, 26))
        MainWindow.setMenuBar(self.menubar)
        self.statusbar = QStatusBar(MainWindow)
        self.statusbar.setObjectName(u"statusbar")
        MainWindow.setStatusBar(self.statusbar)

        self.retranslateUi(MainWindow)

        self.tabWidget.setCurrentIndex(3)
        

        QMetaObject.connectSlotsByName(MainWindow)

        
        
    # setupUi

    def retranslateUi(self, MainWindow):
        MainWindow.setWindowIcon(QIcon('Logo/logo.png'))
        MainWindow.setWindowTitle(QCoreApplication.translate("EES2D-GUI", u"EES2D-GUI", None))
        self.GenerateControl.setText(QCoreApplication.translate("MainWindow", u"Generate Control File", None))
        self.RunSolver.setText(QCoreApplication.translate("MainWindow", u"Run Solver", None))
        self.MeshLabel.setText(QCoreApplication.translate("MainWindow", u"<html><head/><body><p align=\"center\">Mesh File<br/>(SU2 Format)</p></body></html>", None))
        self.MeshFileFinder.setText(QCoreApplication.translate("MainWindow", u"...", None))
        self.tabWidget.setTabText(self.tabWidget.indexOf(self.PreTab), QCoreApplication.translate("MainWindow", u"PRE-PROCESSING", None))
        self.comboBoxSpeedType.setItemText(0, QCoreApplication.translate("MainWindow", u"Mach", None))
        self.comboBoxSpeedType.setItemText(1, QCoreApplication.translate("MainWindow", u"Velocity (m/s)", None))

        self.SpeedTypeLabel.setText(QCoreApplication.translate("MainWindow", u"<html><head/><body><p align=\"center\">Speed Type</p></body></html>", None))
        self.VelocityValueLabel.setText(QCoreApplication.translate("MainWindow", u"<html><head/><body><p align=\"center\">Speed value</p></body></html>", None))
        self.aoaLabel.setText(QCoreApplication.translate("MainWindow", u"Angle of Attack", None))
        self.PressureLabel.setText(QCoreApplication.translate("MainWindow", u"Pressure", None))
        self.TempLabel.setText(QCoreApplication.translate("MainWindow", u"Temperature", None))
        self.ViscosityLabel.setText(QCoreApplication.translate("MainWindow", u"Viscosity", None))
        self.DensityLabel.setText(QCoreApplication.translate("MainWindow", u"Density", None))
        self.GammaLabel.setText(QCoreApplication.translate("MainWindow", u"Gamma", None))
        self.GasCstLabel.setText(QCoreApplication.translate("MainWindow", u"Gas Constant", None))
        self.SpecificHeatLabel.setText(QCoreApplication.translate("MainWindow", u"Specific Heat", None))
        self.VelocityEdit.setHtml(QCoreApplication.translate("MainWindow", u"<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:7.8pt; font-weight:400; font-style:normal;\">\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><br /></p></body></html>", None))
        self.PressureEdit.setText("101325")
        self.TemperatureEdit.setText("288.15")
        self.ViscosityEdit.setText("1.853e-5")
        self.DensityEdit.setText("1.2886")
        self.GammaEdit.setText("1.4")
        self.GasConstantEdit.setText("287.058")
        self.SpecificHeatEdit.setText("1004.7")
        self.AOAunits.setText(QCoreApplication.translate("MainWindow", u"Deg", None))
        self.pressureUnits.setText(QCoreApplication.translate("MainWindow", u"Pa", None))
        self.TempUnits.setText(QCoreApplication.translate("MainWindow", u"K", None))
        self.ViscosityUnits.setText(QCoreApplication.translate("MainWindow", u"Ns/m^2", None))
        self.DensityUnits.setText(QCoreApplication.translate("MainWindow", u"kg/m^3", None))
        self.GasConstantUnits.setText(QCoreApplication.translate("MainWindow", u"J/kg.K", None))
        self.SpecificHeatUnits.setText(QCoreApplication.translate("MainWindow", u"J/kg.K", None))
        self.tabWidget.setTabText(self.tabWidget.indexOf(self.simTab), QCoreApplication.translate("MainWindow", u"SIMULATION", None))
        self.TimeIntegrationLabel.setText(QCoreApplication.translate("MainWindow", u"Time Integration", None))
        self.CflLabel.setText(QCoreApplication.translate("MainWindow", u"CFL", None))
        self.MinResidualLabel.setText(QCoreApplication.translate("MainWindow", u"Maximal Residual", None))
        self.MaxIterLabel.setText(QCoreApplication.translate("MainWindow", u"Max Iterations Number", None))
        self.numThreadsLabel.setText(QCoreApplication.translate("MainWindow", u"Number Of Threads", None))
        self.TimeIntegrationComboBox.setItemText(0, QCoreApplication.translate("MainWindow", u"Runge-Kutta 5", None))
        self.TimeIntegrationComboBox.setItemText(1, QCoreApplication.translate("MainWindow", u"Explicit Euler", None))

        self.tabWidget.setTabText(self.tabWidget.indexOf(self.SolverTab), QCoreApplication.translate("MainWindow", u"SOLVER", None))
        self.PressureFileLabel.setText(QCoreApplication.translate("MainWindow", u"<html><head/><body><p align=\"center\">Pressure File</p></body></html>", None))
        self.ResidualFileLabel.setText(QCoreApplication.translate("MainWindow", u"<html><head/><body><p align=\"center\">Residual File</p></body></html>", None))
        self.OutputFileLabel.setText(QCoreApplication.translate("MainWindow", u"<html><head/><body><p align=\"center\">Output Vtk File</p></body></html>", None))
        self.PressureFileFinder.setText(QCoreApplication.translate("MainWindow", u"...", None))
        self.ResidualFileFinder.setText(QCoreApplication.translate("MainWindow", u"...", None))
        self.OutputFileFinder.setText(QCoreApplication.translate("MainWindow", u"...", None))
        self.tabWidget.setTabText(self.tabWidget.indexOf(self.tab), QCoreApplication.translate("MainWindow", u"POST-PROCESSING", None))
        

    # retranslateUi

Ui = Ui_MainWindow()

myApp = QApplication(sys.argv)
Dialog = QMainWindow()
ui = Ui_MainWindow()
ui.setupUi(Dialog)
Dialog.show()
sys.exit(myApp.exec_())