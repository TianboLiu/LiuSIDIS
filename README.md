# LiuSIDIS
A sidis event generator

# Authors
Tianbo Liu, Zhiwen Zhao, Weizhi Xiong, Haiyan Gao

# How to use it
A quick start to use this generator

Latest version v1.1 (6 January 2017)
## Initialization
* __#include "Lsidis.h"__ //including the head file
* __Lsidis mysidis;__     //create an object
* __mysidis.SetNucleus(double Np, double Nn);__  //set proton and neutron numbers 
* __mysidis.SetHadron(char * hadron);__    //set the final state hadron
* __mysidis.SetInitialState(TLorentzVector l, TLorentzVector P);__ //set initial state kinematics
* __mysidis.SetPDFset("CJ15lo");__ //choose a PDF set
* __mysidis.SetFFset("DSSFFlo");__ //choose a FF set
* __mysidis.SetRange(double Xmin[6], double Xmax[6]);__ //set the generating kinematics range, x, y(Q2), z, Pt, phih, phiS

## Generate event
* __mysidis.GenerateEvent(int mode, int method);__ //generate an event and return the weight, (0, 0) in y space, (0, 1) in Q2 space
* __mysidis.GetVariable(char * variable);__ //return kinematic variable for this event (if it pass a physical condition), e.g. "W", "Q2"
* __mysidis.GetLorentzVector(char * lp);__ //return the final state Lorentz vector "lp" or "Ph"
