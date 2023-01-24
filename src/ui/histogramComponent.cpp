#include "histogramComponent.h"
#include "ofMain.h"

void HistogramComponent::draw()
{
	if (ofxDatGuiFolder::mIsExpanded) {
		ofDrawRectangle(getX(), getY(), getWidth(), getHeight() + getWidth());

        // Find the maximum value in the histograms for normalization
        int redMax = 1, greenMax = 1, blueMax = 1;
        for (int i = 0; i < 256; i++) {
            redMax = max(redMax, redHist[i]);
            greenMax = max(greenMax, greenHist[i]);
            blueMax = max(blueMax, blueHist[i]);
        }

        int bottom = ofxDatGuiFolder::getY() + ofxDatGuiFolder::getWidth() + ofxDatGuiFolder::getHeight();

        //Draw the histograms
        ofSetColor(255, 0, 0, 127);
        for (int i = 0; i < 256; i++) {
            ofDrawLine(i, bottom, i, bottom - redHist[i] * ofxDatGuiFolder::getWidth() / redMax);
        }
        ofSetColor(0, 255, 0, 127);
        for (int i = 0; i < 256; i++) {
            ofDrawLine(i, bottom, i, bottom - greenHist[i] * ofxDatGuiFolder::getWidth() / greenMax);
        }
        ofSetColor(0, 0, 255, 127);
        for (int i = 0; i < 256; i++) {
            ofDrawLine(i, bottom, i, bottom - blueHist[i] * ofxDatGuiFolder::getWidth() / blueMax);
        }
        ofSetColor(255, 255, 255, 255);
	}
	ofxDatGuiFolder::draw();
}

void HistogramComponent::setRedHist(int redHistogram[])
{
    for (int i = 0; i < 256; i++) {
        redHist[i] = redHistogram[i];
    }
}

void HistogramComponent::setGreenHist(int greenHistogram[])
{
    for (int i = 0; i < 256; i++) {
        greenHist[i] = greenHistogram[i];
    }
}

void HistogramComponent::setBlueHist(int blueHistogram[])
{
    for (int i = 0; i < 256; i++) {
        blueHist[i] = blueHistogram[i];
    }
}
