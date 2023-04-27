Scene: Untitled
AntiAliasingType: 1
MSAASamples: 4
EditorCameraClearFlag: 0
ShowGizmos: 1
Entities:
  - ID: 8949066074757219805
    Name: Entity
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
            - Type: 1
              Name: u_AmbientStrength
              Value: 0.5
      Bones:
        []