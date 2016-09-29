#Use this for the red side of Kast and the new, upgraded CCD (as of Sept, 2016)

#Aug 4th, 2016, d46
lambda1=6678.2
pixel1=864
lambda2=6929.47
pixel2=971

#Run from the command line using:
# python tiltencoder.py

###Should not have to edit the numbers below here.

#Pixel to Angstrom conversions taken from here:
# http://mthamilton.ucolick.org/techdocs/instruments/kast/hw_red.html

pixdiff=abs(pixel1-pixel2)*1.30 #1.30 Angstroms/pixel for the 600/7500 grating
pixdiff2=abs(pixel1-pixel2)*0.65 #0.65 Angstroms/pixel for the 1200/5000 grating

lambdadiff=abs(lambda1-lambda2)

print "Wavelength difference between your two chosen lines:"
print lambdadiff, "Ang"
print "Wavelength difference based on your input pixel values:"
print "For the 600/7500 grating:"
print pixdiff , "Ang"
print "For the 1200/5000 grating: " 
print pixdiff2 , "Ang"
print "If the differences match within <10 Angstroms, your grating is correct."

print 
#Now, check that the CCD will cover the wavelength range starting from the dichroic crossover value.
#Want to start at 5500A for the d55 dichroic, and ~4600A for the d46 dichroic.
#The numbers output by the following lines are essentially the first wavelength that will be on the edge of the CCD.
print "For the 600/7500 grating:"
print "These should be equivalent and equal to ~5500 for the d55 dichroic, or ~4650 for the d46:"
print lambda1-pixel1*1.3 #1.30 Angstroms/pixel for the 600/7500 grating
print lambda2-pixel2*1.3 #1.30 Angstroms/pixel for the 600/7500 grating

print "For the 600/7500 grating:"
print "This is the wavelength edge:"

detectorsize = 4096 #This is the number of pixels across the detector in the dimension of dispersion
print lambda1+(detectorsize-pixel1)*1.30 #1.30 Angstroms/pixel for the 600/7500 grating
print lambda2+(detectorsize-pixel2)*1.30 #1.30 Angstroms/pixel for the 600/7500 grating


print "For the 1200/5000 grating:"
print "These should be equivalent and equal to ~5500 for the d55 dichroic, or ~4650 for the d46:"
print lambda1-pixel1*0.65 #0.65 Angstroms/pixel for the 1200/5000 grating
print lambda2-pixel2*0.65 #0.65 Angstroms/pixel for the 1200/5000 grating


