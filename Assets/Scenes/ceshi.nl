Scene: Untitled
AntiAliasingType: 1
MSAASamples: 4
EditorCameraClearFlag: 0
ShowGizmos: 1
Entities:
  - ID: 14086869688480717263
    Name: Entity
    TransformComponent:
      Translation: [3.07579136, 0, 0]
      Rotation: [0, 0, 0]
      Scale: [1, 1, 1]
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
              Name: u_Custom
              Value: ../Assets\Models\Firekeeper\Textures\c1400_hair_a.png
            - Type: 15
              Name: u_Diffuse
              Value: ../Assets\Models\Firekeeper\Textures\c1400_leg_a.png
      Bones:
        []
  - ID: 18432890832883670623
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
              Name: u_Custom
              Value: ../Assets\Models\nanosuit\glass_dif.png
            - Type: 15
              Name: u_Diffuse
              Value: ../Assets\Models\nanosuit\glass_ddn.png
      Bones:
        []