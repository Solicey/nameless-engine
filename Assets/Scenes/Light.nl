Scene: Untitled
AntiAliasingType: 1
MSAASamples: 4
EditorCameraClearFlag: 1
Entities:
  - ID: 17217505591970009995
    Name: Box
    TransformComponent:
      Translation: [0, 0, -6.46709394]
      Rotation: [0, 0, 0]
      Scale: [1, 1, 1]
    ModelRendererComponent:
      ModelPath: ../Assets/Models/Box.obj
      Materials:
        - MaterialName: Material
          ShaderName: Lit.glsl
          ShaderProperties:
            - Type: 6
              Name: u_Color
              Value: [1, 1, 1]
      Bones:
        []
  - ID: 8333281591230251075
    Name: Light
    TransformComponent:
      Translation: [0, 0, 2.09054613]
      Rotation: [0, 0, 0]
      Scale: [1, 1, 1]
    LightComponent:
      LightType: 1
      Color: [0.312431216, 0.937293708, 0.473286957]
      Intensity: 1