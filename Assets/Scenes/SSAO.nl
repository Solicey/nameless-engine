Scene: Untitled
Entities:
  - ID: 17340892127560612441
    Name: Entity
    TransformComponent:
      Translation: [9.61150742, 17.5039272, 1.67278719]
      Rotation: [-1.60169053, -0.00643929327, -0.0377605297]
      Scale: [1.00011766, 1.00015354, 1.00004661]
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
  - ID: 14195874083846681537
    Name: SceneSettings
    TransformComponent:
      Translation: [11.2431879, -8.53512955, -0.306260586]
      Rotation: [-2.0220058, 0, 9.1110096]
      Scale: [9.49250793, 3.17092681, -5.70147371]
    SettingsComponent:
      AntiAliasingType: 0
      EditorCameraClearFlag: 0
      MSAASamples: 4
      ShowGizmos: true
      RenderMode: 1
      ShadowZMult: 10
    PostProcessingComponent:
      Materials:
        - ShaderName: SSAO.glsl
          ShaderProperties:
            - Type: 10
              Name: u_KernelSize
              Value: 64
            - Type: 1
              Name: u_Radius
              Value: 6.19999981
            - Type: 1
              Name: u_Bias
              Value: 0.00999999978
            - Type: 1
              Name: u_EnableSRGBCorrection
              Value: 0
        - ShaderName: Blur.glsl
          ShaderProperties:
            []
        - ShaderName: EditorOutline.glsl
          ShaderProperties:
            []
  - ID: 16939305440333559909
    Name: Cam
    TransformComponent:
      Translation: [8.45195293, 7.97469664, 16.3556309]
      Rotation: [0, 0, 0]
      Scale: [1, 1, 1]
    CameraComponent:
      FixedAspectRatio: false
      ClearColor: [0.192000002, 0.301999986, 0.474999994, 1]
      ProjectionType: 1
      OrthoSize: 10
      OrthoFar: 100
      OrthoNear: 0.00999999978
      PerspFOV: 0.785398185
      PerspFar: 1000
      PerspNear: 0.00999999978
      ViewportWidth: 1280
      ViewportHeight: 720
      ClearFlagType: 0
    PostProcessingComponent:
      Materials:
        - ShaderName: SSAO.glsl
          ShaderProperties:
            - Type: 10
              Name: u_KernelSize
              Value: 64
            - Type: 1
              Name: u_Radius
              Value: 3
            - Type: 1
              Name: u_Bias
              Value: 0
            - Type: 1
              Name: u_EnableSRGBCorrection
              Value: 0
        - ShaderName: Blur.glsl
          ShaderProperties:
            []