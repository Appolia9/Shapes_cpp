The abstract class **"shape"** has been declared in the program. All specific shapes — line, rectangle, etc. — are derived from this class. 
The "shape" class defines the actions required for all shapes.
It also creates a common list of all declared shapes, which can be used by the drawing function when displaying shapes on the screen.
In the "shape" class, a set of member functions has been declared that must support all shapes so that pictures can be created from them.
These are functions for drawing, moving, and resizing shapes, as well as functions that return the coordinates of all the extreme points of the shape
along which they can be joined.\
There are also two additional classes that specify the properties of shapes. 
Some shapes can be rotated. For such shapes, there is a rotatable base class derived from shape.
For other shapes, a reflection operation is possible relative to the horizontal or vertical axis. 
These shapes can be built based on the reflective class. If both are needed for a shape, then it must be derived from both classes.\
The **"shape"** class is the core of the snape shape library.h. There is also a screen support library.h, in which the screen dimensions are defined,
the concept of a point is introduced and the screen utilities are listed, which are then specified in shape.h. 
For simplicity and versatility, screen operation is implemented as the formation and line-by-line output of a matrix of symbols.\
When the program starts, the announced collection of shapes is first displayed on the screen.
Then the result of the rotation/reflection of some shapes is demonstrated as a preparation for their use. Then, using the attachment function, 
the figures move and form a given picture: a face in a hat. Rectangles, lines, and dots are used for drawing. The physiognomy is a custom figure.\
The picture is complemented by a semicircle with a cross as a separate figure. Horns and a tie consist of this figure.
