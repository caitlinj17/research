#Use this for the blue side of Kast.

#March 17th, 2016, d46:
lambda1=3888.65 
pixel1=1077
lambda2=4046.56
pixel2=1320

#March 17th, 2016, d55:
#lambda1=4358.33
#pixel1=1084

#lambda2=4046.56
#pixel2=774

#Run from the command line using:
# python XYstage.py

##Shouldn't have to edit below here:
#The pixel to angstrom conversions are taken from here:
# http://mthamilton.ucolick.org/techdocs/instruments/kast/hw_blue.html

gr3_angperpixel = 0.63
gr2_angperpixel = 1.02

print "Wavelength difference between your two chosen lines:"
print abs(lambda1-lambda2), "Ang"
print "Wavelength difference based on your input pixel values:"
print "For the d55:"
print abs(pixel1-pixel2)*gr2_angperpixel, "Ang"
print "For the d46:"
print abs(pixel1-pixel2)*gr3_angperpixel, "Ang" 
print "If the differences match within <10 Angstroms, your grism is correct."
print
#Now, check that the wavelengths up to the dichroic crossover will be covered by the CCD.
#Want to go to 5500A for the d55 dichroic, and ~4600A for the d46 dichroic.
#The numbers output by the following lines are essentially the last wavelength that will be on the edge of the CCD.
print "For using the d55:"
print "Iterate the XY stage until these are equivalent and ~5500: "
print lambda1+(2048-pixel1)*gr2_angperpixel #2048 is the total number of pixels in the CCD in the dimension that the wavelength covers.
print lambda2+(2048-pixel2)*gr2_angperpixel

print "For using the d46:"
print "Iterate the XY stage until these are equivalent and ~4600: "
print lambda1+(2048-pixel1)*gr3_angperpixel
print lambda2+(2048-pixel2)*gr3_angperpixel
