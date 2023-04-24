Scene: Untitled
AntiAliasingType: 1
MSAASamples: 4
EditorCameraClearFlag: 0
ShowGizmos: 1
Entities:
  - ID: 18108633370218257906
    Name: Light
    TransformComponent:
      Translation: [0.959417641, 4.18561888, -2.12181497]
      Rotation: [0.387549549, -0.0117626935, -0.00480127987]
      Scale: [1, 0.999999762, 0.99999994]
    LightComponent:
      LightType: 0
      Color: [0.841584146, 0.376118213, 0.324968129]
      Intensity: 1
  - ID: 7682593134752729058
    Name: Box
    TransformComponent:
      Translation: [0, 0, 0]
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