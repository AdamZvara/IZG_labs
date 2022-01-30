import bpy
import bmesh
import mathutils
import math
from math import radians

class EP_PT_ExercisePanel(bpy.types.Panel):
    bl_space_type = "VIEW_3D"
    bl_region_type = "UI"
    bl_context = "objectmode"
    bl_category = "IZG Exercise"
    bl_label = "3D Transformations"

    def draw(self, context):
        col = self.layout.column(align=True)
        col.prop(context.scene, "login")
        col.prop(context.scene, "modelmatrix")
        col.prop(context.scene, "viewmatrix")
        col.operator("mesh.generate", text="Generate")
        col.operator("view.align", text="Align")
        
class Align(bpy.types.Operator):
    bl_idname = "view.align"
    bl_label = "Aligns the viewport to worldspace default position"
    bl_description = "Aligns the viewport to worldspace default position"
    bl_options = {"UNDO"}
    
    def invoke(self, context, event):
        context.space_data.region_3d.view_location = (0.0, 0.0, -5.0)
        context.space_data.region_3d.view_rotation = (0.0, 1.0, 0.0, 0.0)
        return {"FINISHED"}

class Generate(bpy.types.Operator):
    bl_idname = "mesh.generate"
    bl_label = "Generates the scene"
    bl_description = "Generates the scene"
    bl_options = {"UNDO"}

#The important part!!!!
###############################################
#Go through the code, read comments and look for TODO sections.
#all necessary matrix or vector operations can be found at https://docs.blender.org/api/current/mathutils.html

    #in this function a view matrix is created according to camera parameters and coordinates
    def createView():
        
        #TODO define correct values for the important parameters (the answer is not 42 this time):
        #######################
        #position of camera, find it in the code or in Blender UI after generating the scene
        eyePosition = mathutils.Vector((0.0, -10.0, 0.0))
        #point where the camera looks at (center of the scene)
        centerPosition = mathutils.Vector((0.0, 0.0, 0.0))
        #defines where is "up" in your scene, along Z axis
        upVector =  mathutils.Vector((0.0, 0.0, 1.0))      
        #here a new coordinate system is created relative to the camera, the system transforms the original one by the camera transformation
        #vector pointing from the center to the camera (eyePosition) will define the new Z aka depth, should be rotated around Z to compensate camera's Y rotation
        Z = mathutils.Vector((0.0, -10.0, 0.0))
        Z.normalize()
        #Y will be pointing up in the view space
        Y = mathutils.Vector((0.0, 0.0, 1.0))
        #X has to be perpendicular to Z and Y
        X = mathutils.Vector(Y.cross(Z))
        #######################
        
        #combining all to the matrix
        X.normalize()
        Y.normalize()
        view = mathutils.Matrix.Identity(4)
        view[0][0] = X.x
        view[1][0] = X.y
        view[2][0] = X.z
        view[3][0] = -X.dot(eyePosition)
        view[0][1] = Y.x
        view[1][1] = Y.y
        view[2][1] = Y.z
        view[3][1] = -Y.dot(eyePosition)
        view[0][2] = Z.x
        view[1][2] = Z.y
        view[2][2] = Z.z
        view[3][2] = -Z.dot(eyePosition)
        view[0][3] = 0
        view[1][3] = 0
        view[2][3] = 0
        view[3][3] = 1.0
        
        #rotates the letters so it looks like the view from camera - change 30 degrees if needed
        view = mathutils.Matrix.Rotation(math.radians(13), 4, "Z") @ view

        return view
       
    #this function returns modelview matrix for specific object
    def createTransformation(object, view):
        #this variable contains the final 4x4 transformation matrix that will be applied to all vertices of the model
        transformation = mathutils.Matrix.Identity(4)
        #view matrix created in createView() is available in view variable
        #transformation matrices: 
        #translationMatrix = mathutils.Matrix.Translation((x, y, z)) etc. 
        #matrix multiplication: m1 @ m2 (the order matters, choose the right side...of the force)

        #objectNumber variable contains the id of current object (the index of character in your login string)
        objectNumber = int(object.name)
        
        #TODO: create the transformation matrix for each object
        #the desired transformation will be described at the beginning of the exercise (ask the teacher if not)V
        
        #create translation matrix, dependant on each object (objectNumber), current translation is by vector (1,0,0)
        translationMatrix = mathutils.Matrix.Translation((objectNumber * 1 - 3.5,0,0))

        #rotate so the letters are in reversed order
        rotationMatrix1 = mathutils.Matrix.Rotation(math.radians(180), 4, "Z")

        #create rotation matrix, same for each object, rotation is by 90 degrees around x axis
        rotationMatrix2 = mathutils.Matrix.Rotation(math.radians(90), 4, "X")

        #multiply matrices together (last is the first to apply) - first translate, then rotate
        #TODO make dependant on number of letters
        transformation = rotationMatrix2 @ rotationMatrix1 @ translationMatrix

        transformation = view @ transformation
        return transformation
    
    #here the transformation matrix is applied on model's vertices    
    def applyTransformation(transformation, object):
        for vertex in object.data.vertices:

            #TODO: apply the transformation on vertices, mathutils.Vector is just a cast to be able to multiply it by a matrix   
            vertex.co = mathutils.Vector(vertex.co)
            vertex.co = transformation @ vertex.co
            
        return
#have fun and don't be afraid to ask....
#The important thing is not to stop questioning. - Albert Einstein
###############################################


    def invoke(self, context, event):
        #clear scene
        for o in bpy.context.scene.objects:
            o.select_set(True)
        bpy.ops.object.delete()

        if not context.scene.login:
            return {"FINISHED"}
        
        #generate objects
        i=0
        for character in context.scene.login:          
            bpy.ops.object.text_add()
            ob=bpy.context.object
            ob.data.body = character
            ob.name=str(i)
            i+=1
          
        #3D    
        textObjs = [ o for o in bpy.data.objects if o.type == 'FONT' ]
        for o in textObjs:
            o.data.extrude = 0.2
            o.data.bevel_depth = 0.03
            o.select_set(True)
        bpy.ops.object.convert(target="MESH")
        
        #centering
        bpy.ops.object.origin_set(type='ORIGIN_GEOMETRY', center='BOUNDS')
        for o in textObjs:
            o.location = mathutils.Vector((0.0, 0.0, 0.0))
        
        #transformation      
        view = mathutils.Matrix.Identity(4)
        if context.scene.viewmatrix:
            view = Generate.createView()
            print(view)
        objects = [ o for o in bpy.data.objects if o.type == 'MESH' ]
        for o in textObjs:
            transformation = Generate.createTransformation(o, view)
            if not context.scene.modelmatrix:
                transformation = view
            Generate.applyTransformation(transformation, o)

        #recalculate normals
        bpy.ops.object.mode_set(mode='EDIT')
        bpy.ops.mesh.normals_make_consistent(inside=False)
        bpy.ops.object.editmode_toggle()
        
        bpy.ops.object.camera_add(location=(0.0, -10.0, 0.0),rotation=(radians(90.0), radians(-13.0), 0.0))
            
        return {"FINISHED"}


def register():
    bpy.utils.register_class(EP_PT_ExercisePanel)
    bpy.utils.register_class(Generate)
    bpy.utils.register_class(Align)
    bpy.types.Scene.login = bpy.props.StringProperty(name="Login", description="Write your own login")
    bpy.types.Scene.modelmatrix = bpy.props.BoolProperty(name="Apply model matrices", description="Enables the model matrix multiplication part")
    bpy.types.Scene.viewmatrix = bpy.props.BoolProperty(name="Apply view matrix", description="Enables the model view matrix multiplication part")
    
def unregister():
    bpy.utils.unregister_class(EP_PT_ExercisePanel)
    bpy.utils.unregister_class(Generate)
    bpy.utils.register_class(Align)
    del bpy.types.Scene.lfType 
    
if __name__ == "__main__" :
    register()