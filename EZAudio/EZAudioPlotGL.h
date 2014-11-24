//
//  EZAudioPlotGL.h
//  EZAudio
//
//  Created by Syed Haris Ali on 11/22/13.
//  Copyright (c) 2013 Syed Haris Ali. All rights reserved.
//
//  Permission is hereby granted, free of charge, to any person obtaining a copy
//  of this software and associated documentation files (the "Software"), to deal
//  in the Software without restriction, including without limitation the rights
//  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//  copies of the Software, and to permit persons to whom the Software is
//  furnished to do so, subject to the following conditions:
//
//  The above copyright notice and this permission notice shall be included in
//  all copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
//  THE SOFTWARE.

#import "TargetConditionals.h"
#import "EZPlot.h"
#import <GLKit/GLKit.h>

@class EZAudioPlotGLKViewController;

#pragma mark - Enumerations
/**
 Constant drawing types wrapping around the OpenGL equivalents. In the audio drawings the line strip will be the stroked graph while the triangle will provide the filled equivalent.
 */
typedef NS_ENUM(NSUInteger,EZAudioPlotGLDrawType){
  /**
   *  Maps to the OpenGL constant for a line strip, which for the audio graph will correspond to a stroked drawing (no fill).
   */
  EZAudioPlotGLDrawTypeLineStrip     = GL_LINE_STRIP,
  /**
   *  Maps to the OpenGL constant for a triangle strip, which for the audio graph will correspond to a filled drawing.
   */
  EZAudioPlotGLDrawTypeTriangleStrip = GL_TRIANGLE_STRIP
};

#pragma mark - Structures
/**
 A structure describing a 2D point (x,y) in space for an audio plot.
 */
typedef struct {
  GLfloat x;
  GLfloat y;
} EZAudioPlotGLPoint;

/**
 EZAudioPlotGL is a subclass of either the EZPlot on iOS or an NSOpenGLView on OSX. I apologize ahead of time for the weirdness in the docs for this class, but I had to do a bit of hackery to get a universal namespace for something works on both iOS and OSX without any additional components. The EZAudioPlotGL provides an the same utilities and interface as the EZAudioPlot with the added benefit of being GPU-accelerated. This is the recommended plot to use on iOS devices to get super fast real-time drawings of audio streams. For the methods and properties below I've included notes on the bottom just indicating which OS they correspond to. In most (if not all) use cases you can just refer to the EZPlot documentation to see which custom properties can be setup. There update function is the same as the EZPlot as well: `updateBuffer:withBufferSize:`
 */
@interface EZAudioPlotGL : EZPlot


#pragma mark - Adjust Resolution
///-----------------------------------------------------------
/// @name Adjusting The Resolution
///-----------------------------------------------------------

/**
 Sets the length of the rolling history display. Can grow or shrink the display up to the maximum size specified by the kEZAudioPlotMaxHistoryBufferLength macro. Will return the actual set value, which will be either the given value if smaller than the kEZAudioPlotMaxHistoryBufferLength or kEZAudioPlotMaxHistoryBufferLength if a larger value is attempted to be set.
 @param  historyLength The new length of the rolling history buffer.
 @return The new value equal to the historyLength or the kEZAudioPlotMaxHistoryBufferLength.
 */
-(int)setRollingHistoryLength:(int)historyLength;

/**
 Provides the length of the rolling history buffer
 *  @return An int representing the length of the rolling history buffer
 */
-(int)rollingHistoryLength;

#pragma mark - Shared Methods
///-----------------------------------------------------------
/// @name Clearing The Plot
///-----------------------------------------------------------

/**
 Clears all data from the audio plot (includes both EZPlotTypeBuffer and EZPlotTypeRolling)
 */
-(void)clear;

///-----------------------------------------------------------
/// @name Shared OpenGL Methods
///-----------------------------------------------------------
/**
 Converts a float array to an array of EZAudioPlotGLPoint structures that hold the (x,y) values the OpenGL buffer needs to properly plot its points.
 @param graph       A pointer to the array that should hold the EZAudioPlotGLPoint structures.
 @param graphSize   The size (or length) of the array with the EZAudioPlotGLPoint structures.
 @param drawingType The EZAudioPlotGLDrawType constant defining whether the plot should interpolate between points for a triangle strip (filled waveform) or not for a line strip (stroked waveform)
 @param buffer      The float array holding the audio data
 @param bufferSize  The size of the float array holding the audio data
 @param gain        The gain (always greater than 0.0) to apply to the amplitudes (y-values) of the graph. Y-values can only range from -1.0 to 1.0 so any value that's greater will be rounded to -1.0 or 1.0.
 */
+(void)fillGraph:(EZAudioPlotGLPoint*)graph
   withGraphSize:(UInt32)graphSize
  forDrawingType:(EZAudioPlotGLDrawType)drawingType
      withBuffer:(float*)buffer
  withBufferSize:(UInt32)bufferSize
        withGain:(float)gain;

/**
 Determines the proper size of a graph given a EZAudioPlotGLDrawType (line strip or triangle strip) and the size of the incoming buffer. Triangle strips require interpolating between points so the buffer becomes 2*bufferSize
 @param drawingType The EZAudioPlotGLDraw type (line strip or triangle strip)
 @param bufferSize  The size of the float array holding the audio data coming in.
 @return A Int32 representing the proper graph size that should be used to account for any necessary interpolating between points.
 */
+(UInt32)graphSizeForDrawingType:(EZAudioPlotGLDrawType)drawingType
                  withBufferSize:(UInt32)bufferSize;

@end
