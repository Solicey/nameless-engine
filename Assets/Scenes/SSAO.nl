Scene: Untitled
Entities:
  - ID: 14195874083846681537
    Name: SceneSettings
    TransformComponent:
      Translation: [0, 0, 0]
      Rotation: [0, 0, 0]
      Scale: [1, 1, 1]
    SettingsComponent:
      AntiAliasingType: 0
      EditorCameraClearFlag: 0
      MSAASamples: 4
      ShowGizmos: true
      RenderMode: 1
    PostProcessingComponent:
      Materials:
        - ShaderName: SSAO.glsl
          ShaderProperties:
            []
        - ShaderName: Blur.glsl
          ShaderProperties:
            []
  - ID: 5062354456308492637
    Name: Entity
    TransformComponent:
      Translation: [6.75650215, 7.37207031, -3.22419953]
      Rotation: [0, 0, 0]
      Scale: [1, 1, 1]
    ModelRendererComponent:
      ModelPath: ../Assets/Models/Box.obj
      Materials:
        - MaterialName: Material
          ShaderName: Default.glsl
          ShaderProperties:
            []
      Bones:
        []
  - ID: 1588043360304861613
    Name: Entity
    TransformComponent:
      Translation: [9.75598907, 10.3915157, -7.49657726]
      Rotation: [0.0722035095, -0.502465189, -0.0554177351]
      Scale: [6, 6, 6]
    ModelRendererComponent:
      ModelPath: ../Assets/Models/Sphere.obj
      Materials:
        - MaterialName: None
          ShaderName: Default.glsl
          ShaderProperties:
            []
      Bones:
        []
  - ID: 6212189539976217936
    Name: Entity
    TransformComponent:
      Translation: [10.3653955, 4.36548424, 2.08564043]
      Rotation: [-1.53933918, 0.0024690209, -3.05207276]
      Scale: [1.00010133, 1.00011683, 1.00006211]
    ModelRendererComponent:
      ModelPath: ../Assets/Models/nanosuit/nanosuit.obj
      Materials:
        - MaterialName: Glass
          ShaderName: Default.glsl
          ShaderProperties:
            []
        - MaterialName: Leg
          ShaderName: Default.glsl
          ShaderProperties:
            []
        - MaterialName: Helmet
          ShaderName: Default.glsl
          ShaderProperties:
            []
        - MaterialName: Hand
          ShaderName: Default.glsl
          ShaderProperties:
            []
        - MaterialName: Body
          ShaderName: Default.glsl
          ShaderProperties:
            []
        - MaterialName: Arm
          ShaderName: Default.glsl
          ShaderProperties:
            []
      Bones:
        []
  - ID: 16109096969228851717
    Name: Entity
    TransformComponent:
      Translation: [4.41631699, 4.93779087, -6.20138359]
      Rotation: [0, 0, 0]
      Scale: [1, 1, 1]
    ModelRendererComponent:
      ModelPath: ../Assets/Models/Box.obj
      Materials:
        - MaterialName: Material
          ShaderName: Default.glsl
          ShaderProperties:
            []
      Bones:
        []