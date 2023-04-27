Scene: Untitled
AntiAliasingType: 1
MSAASamples: 4
EditorCameraClearFlag: 0
ShowGizmos: 1
Entities:
  - ID: 88934781535075632
    Name: Entity
    TransformComponent:
      Translation: [0, 0, 0]
      Rotation: [0, 0, 0]
      Scale: [1, 1, 1]
    ModelRendererComponent:
      ModelPath: ../Assets/Models/Box.obj
      Materials:
        - MaterialName: Material
          ShaderName: NormalVisualize.glsl
          ShaderProperties:
            - Type: 6
              Name: u_Color
              Value: [1, 0.419141889, 0.419141889]
            - Type: 1
              Name: u_Magnitude
              Value: 0.400000006
      Bones:
        []