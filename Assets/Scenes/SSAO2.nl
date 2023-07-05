Scene: Untitled
Entities:
  - ID: 1394329101617413596
    Name: Light
    TransformComponent:
      Translation: [-4.42822409, 13.9251213, 0.853923202]
      Rotation: [0, 0.0283682905, 0]
      Scale: [1, 1, 1]
    LightComponent:
      LightType: 1
      Color: [0.755775571, 0.610340059, 0.379134923]
      Intensity: 2.8900001
      Attenuation: [1, 0.699999988, 1.79999995]
  - ID: 13074882754255396138
    Name: SceneSettings
    TransformComponent:
      Translation: [2.32245946, 17.5441628, 5.06910038]
      Rotation: [0.701999664, 0, 5.75400066]
      Scale: [-1.5653677, 11.0311069, -1.57877851]
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
            - Type: 10
              Name: u_KernelSize
              Value: 64
            - Type: 1
              Name: u_Radius
              Value: 40.5
            - Type: 1
              Name: u_Threshold
              Value: 0
        - ShaderName: Blur.glsl
          ShaderProperties:
            []
        - ShaderName: DeferredShading(SSAO).glsl
          ShaderProperties:
            []
  - ID: 14454222724462806668
    Name: Entity
    TransformComponent:
      Translation: [0, 0, 0]
      Rotation: [8.38589926e-08, -0.504291475, -6.56757919e-08]
      Scale: [1, 1, 1]
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
  - ID: 5115668116731516499
    Name: Cam
    TransformComponent:
      Translation: [-2.32830644e-10, 10.0026264, 19.0071144]
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
              Name: u_Threshold
              Value: 0
        - ShaderName: Blur.glsl
          ShaderProperties:
            []
        - ShaderName: DeferredShading(SSAO).glsl
          ShaderProperties:
            []
  - ID: 679017688966366070
    Name: Entity
    TransformComponent:
      Translation: [-5.02937269, 3.30307927e-07, -2.77564955]
      Rotation: [8.89495126e-08, 0.599865913, 2.50589824e-08]
      Scale: [0.999999642, 1, 0.999999642]
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