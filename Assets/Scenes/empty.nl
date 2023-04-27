Scene: Untitled
AntiAliasingType: 1
MSAASamples: 4
EditorCameraClearFlag: 1
ShowGizmos: 1
Entities:
  - ID: 17975165306985353272
    Name: Entity
    TransformComponent:
      Translation: [-5.72303009, 0, 0]
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
              Value: [0.0273176562, 0.68976897, 0]
            - Type: 1
              Name: u_AmbientStrength
              Value: 6
            - Type: 6
              Name: u_Test
              Value: [1, 0, 0]
      Bones:
        []
  - ID: 8413354406637048712
    Name: Entity
    TransformComponent:
      Translation: [0, 0, 0]
      Rotation: [0, 0, 0]
      Scale: [1, 1, 1]
    ModelRendererComponent:
      ModelPath: ../Assets/Models/Sphere.obj
      Materials:
        - MaterialName: None
          ShaderName: Lit.glsl
          ShaderProperties:
            - Type: 6
              Name: u_Color
              Value: [0.894389451, 0, 0]
            - Type: 1
              Name: u_AmbientStrength
              Value: 0
            - Type: 6
              Name: u_Test
              Value: [0.480933279, 0.735973597, 0]
      Bones:
        []