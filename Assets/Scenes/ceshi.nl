Scene: Untitled
AntiAliasingType: 1
MSAASamples: 4
EditorCameraClearFlag: 1
ShowGizmos: 1
Entities:
  - ID: 16594911826917458950
    Name: Entity
    TransformComponent:
      Translation: [2.91387177, 0, -2.32612157]
      Rotation: [0, 0, 0]
      Scale: [1.5, 1, 1.35275304]
    ModelRendererComponent:
      ModelPath: ../Assets/Models/Box.obj
      Materials:
        - MaterialName: Material
          ShaderName: TexUnlit.glsl
          ShaderProperties:
            - Type: 6
              Name: u_Color
              Value: [1, 1, 1]
            - Type: 15
              Name: u_Diffuse
              Value: ../Assets\Models\vampire\test\textures\Vampire_specular.png
      Bones:
        []
  - ID: 14377977088945513866
    Name: Entity
    TransformComponent:
      Translation: [0, 0, 0]
      Rotation: [0, 0, 0]
      Scale: [1, 1, 1]
    ModelRendererComponent:
      ModelPath: ../Assets/Models/Sphere.obj
      Materials:
        - MaterialName: None
          ShaderName: TexUnlit.glsl
          ShaderProperties:
            - Type: 6
              Name: u_Color
              Value: [1, 1, 1]
            - Type: 15
              Name: u_Diffuse
              Value: ../Assets\Models\nanosuit\arm_showroom_ddn.png
      Bones:
        []