#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

#include "TCanvas.h"
#include "TLegend.h"
#include "TH1D.h"

void SimulateLCProb2(){

////////////////Configuration things:
const int ndays=109; //Number of days in the light curve.
const int nLC=10000000;//Number of light curves to simulate.
double meaninput =4.40953e-08; // the mean flux of your LC
//double meaninput =0.0;// the mean flux of your LC
double sigmainput =7.46773e-08; //The rms of the LC 
//double sigmainput =1.0; //The rms of the LC 
bool doconsecutive = true; //whether or not to calculate probability for consecutive days flaring.
double threshold = 3.2; //in units of RMS.

//////Spit back out the parameters:
cout << "~~~~~Let's Simulate Your Light Curve!~~~~~" << endl;
cout << "Each light curve is " << ndays << " days long." << endl;
cout << "You are making " << nLC << " light curves. " << endl;
cout << "Each light curve is drawn from a Gaussian with: " << endl;
cout << "mean = " << meaninput << endl;
cout << "sigma = " << sigmainput << endl;
if (doconsecutive)
cout << "You are also calculating the probability of consecutive flares." << endl;

if (!doconsecutive)
cout << "You are NOT calculating the probability of consecutive flares." << endl;

/////////////////Run things.

//This array is your light curve:
double lightcurve[ndays];

//Your random number generator:
//for(int i=0; i<100; i++){
TRandom3 *rand = new TRandom3(41669434);

//Your function for generating random numbers:
double norm=1.0/(sigmainput*pow(2.0*3.1415926,0.5)); //Normalization on your LC histogram
TF1 *LCFunc = new TF1("LCFunc","[0]*exp(-0.5*((x-[1])/[2])^2)", meaninput-10.0*sigmainput,meaninput+10.0*sigmainput);
LCFunc->SetParameter(0, norm); //Normalization
LCFunc->SetParameter(1, meaninput); //Mean
LCFunc->SetParameter(2, sigmainput); //Sigma


	TH1F *h1 = new TH1F("h1","rand->Gaus(mean,sigma)",100,meaninput-8.0*sigmainput,meaninput+8.0*sigmainput);
	TH1F *h2 = new TH1F("h2","LCfunction->GetRandom()",100,meaninput-8.0*sigmainput,meaninput+8.0*sigmainput);
	TH1F *h3 = new TH1F("h3","All Light Curve Values (expect ndays*nLC entries)",100,meaninput-8.0*sigmainput,meaninput+8.0*sigmainput);
	TH1F *hmeans = new TH1F("hmeans","Histogram of the Calculated Means",100,meaninput-3.0*sigmainput,meaninput+3.0*sigmainput);
	TH1F *hsigmas = new TH1F("hsigmas","Histogram of the Calculated Sigmas",100,sigmainput-3.0*sigmainput,sigmainput+3.0*sigmainput);
//Initialize how many lightcurves have the specified number of days above the threshold:
int oneday=0, threedays=0, twodays=0, fourdays=0;
int threedays_consecutive=0, twodays_consecutive=0;
int oneday_input=0, threedays_input=0, twodays_input=0, fourdays_input=0;

//Loop over the LC:

for(int NN=0;NN<nLC; NN++){

	if(NN%100000==0)
	cout << "Working on LC number: " << NN << endl;	
	
	//Initialize the mean of the light curve:
	double mean=0;
	//Fill the light curve array:
	//cout <<"LC #" <<NN ;
	for(int i=0;i<ndays;i++){
//		lightcurve[i]=rand->Gaus(meaninput,sigmainput); //Don't use this one. It gives the same values every time.
		lightcurve[i]=LCFunc->GetRandom();
		mean=mean+lightcurve[i];
		h3->Fill(lightcurve[i]);
		//cout<< ", " << lightcurve[i];
	}
//cout << " " << endl;
	//h1->Fill(rand->Gaus(meaninput,sigmainput));
	//h2->Fill(LCFunc->GetRandom());

	//Calculate the actual mean of the light curve:
	mean = mean/(ndays*1.0);
	hmeans->Fill(mean);

	//Calculate the sample standard deviation of the light curve:
	double RMS=0;
	for(int i=0;i<ndays;i++){
		RMS=RMS+pow((lightcurve[i]-mean),2.0);
	}
	RMS = RMS/((ndays-1)*1.0);
	RMS=pow(RMS,0.5);
	hsigmas->Fill(RMS);

	//cout <<"LC #"<< NN << " mean=" << mean << " RMS=" << RMS<< endl;
	//Now check How many points are above a certain threshold.
	int nabove=0;
	int nabove_input=0;

	for(int i=0;i<ndays;i++){
		if ((lightcurve[i] - meaninput)/sigmainput >= threshold ){
			nabove_input++;}
		if ((lightcurve[i] - mean)/RMS >= threshold ){
			nabove++;}
	}

if(nabove == 1)
oneday++;

if(nabove == 2)
twodays++;

if(nabove ==3){
threedays++;
//cout << "Light curve #: " << NN << " nabove=" << nabove << endl;
}

if(nabove == 4)
fourdays++;

////////Using the input mean and sigma:
if(nabove_input == 1)
oneday_input++;

if(nabove_input == 2)
twodays_input++;

if(nabove_input ==3){
threedays_input++;
//cout << "Light curve #: " << NN << " nabove=" << nabove << endl;
}

if(nabove_input == 4)
fourdays_input++;

//Now count the light curves with consecutive flares.
if (doconsecutive){
	if(nabove == 2){
	//twodays++;
	for(int i=0;i<ndays-1;i++){
		double diff1=(lightcurve[i] - mean)/RMS;
		double diff2=(lightcurve[i+1] - mean)/RMS;
		if ((diff1 >= threshold) &&(diff2 >= threshold) ){
			twodays_consecutive++;
			break;}
		}
	}

	if(nabove ==3){
	for(int i=0;i<ndays-2;i++){
		double diff1=(lightcurve[i] - mean)/RMS;
		double diff2=(lightcurve[i+1] - mean)/RMS;
		double diff3=(lightcurve[i+2] - mean)/RMS;
		if ((diff1 >= threshold) &&(diff2 >= threshold)&&(diff3 >= threshold)  ){
			threedays_consecutive++;
			break;}
		}
	}
	}


}//End of the LC loop


//cout << "two days: " << twodays << " three days: " << threedays << endl;
cout << " " << endl;

//Calculate P value for the given threshold:
double P=(1.0-TMath::Erf(threshold/sqrt(2)))/2.0;

cout << "For threshold of " << threshold << "sigma the calculated P value is: " << P << endl;
cout << " " <<endl;
cout << "# of Light Curves with 1 day above: " << oneday <<endl;
cout << "Simulated Probability: " << oneday / (nLC *1.0) << endl;
cout << "Simulated Probability, using input values: " << oneday_input / (nLC *1.0) << endl;
cout << "Calculated Probability: " << TMath::Binomial(ndays, 1) * pow(P,1)*pow(1.0-P,ndays*1.0 - 1.0)<<endl;
cout <<" "<< endl;
cout << "# of Light Curves with 2 days above: " << twodays <<endl;
cout << "Simulated Probability: " << twodays / (nLC *1.0) << endl;
cout << "Simulated Probability, using input values: " << twodays_input / (nLC *1.0) << endl;
cout << "Calculated Probability: " << TMath::Binomial(ndays, 2) * pow(P,2)*pow(1.0-P,ndays-2)<<endl;
cout <<" "<< endl;
cout << "# of Light Curves with 3 days above: " << threedays <<endl;
cout << "Simulated Probability: " << threedays / (nLC *1.0) << endl;
cout << "Simulated Probability, using input values: " << threedays_input / (nLC *1.0) << endl;
cout << "Calculated Probability: " << TMath::Binomial(ndays, 3) * pow(P,3)*pow(1.0-P,ndays-3)<<endl;
cout <<" "<< endl;
cout << "# of Light Curves with 4 days above: " << fourdays <<endl;
cout << "Simulated Probability: " << fourdays / (nLC *1.0) << endl;
cout << "Simulated Probability, using input values: " << fourdays_input / (nLC *1.0) << endl;
cout << "Calculated Probability: " << TMath::Binomial(ndays, 4) * pow(P,4)*pow(1.0-P,ndays-4)<< endl;

cout <<" "<< endl;
if (doconsecutive){
cout << "---Consecutive Probabilities:---"<< endl;
cout << "# of Light Curves with 2 consecutive days above threshold: " << twodays_consecutive <<endl;
cout << "Percent of light curves with 2 flare days that are consecutive: " <<twodays_consecutive*100.0 / twodays*1.0 <<endl;
cout << "Expected Percent: " << ((ndays-1)*100.0)/(TMath::Binomial(ndays, 2) * 1.0)<< endl;
cout << "Simulated Probability: " << twodays_consecutive / (nLC *1.0) << endl;
cout << endl;
cout << "# of Light Curves with 3 consecutive days above threshold: " << threedays_consecutive <<endl;
cout << "Percent of light curves with 3 flare days that are consecutive: " <<threedays_consecutive*100.0 / threedays*1.0 <<endl;
cout << "Expected Percent: " << ((ndays-2)*100.0)/(TMath::Binomial(ndays, 3) * 1.0)<< endl;
cout << "Simulated Probability: " << threedays_consecutive / (nLC *1.0) << endl;
}

/*TCanvas *canvas1 = new TCanvas("Canvas1","",1000,800);
h1->Draw();*/

/*TCanvas *canvas2 = new TCanvas("Canvas2","",1000,800);
h2->Draw();*/

TCanvas *canvas3 = new TCanvas("Canvas3","",1000,800);
h3->Draw();

TCanvas *canvas4 = new TCanvas("Canvas4","",1000,800);
hmeans->Draw();

TCanvas *canvas5 = new TCanvas("Canvas5","",1000,800);
hsigmas->Draw();


}




