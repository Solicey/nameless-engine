Scene: Untitled
Entities:
  - ID: 13074890163083427898
    Name: Entity
    TransformComponent:
      Translation: [0, 0, 0]
      Rotation: [0, 0, 0]
      Scale: [1, 1, 1]
    SpriteRendererComponent:
      Color: [1, 1, 1, 1]
      Reaction: 2
      Path: ../Assets\Models\Bonfire\o000100_01_a.png
  - ID: 931269729254786564
    Name: Entity
    TransformComponent:
      Translation: [-0.00254520774, 0, 6.14208078]
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
      ClearFlagType: 1
    PostProcessingComponent:
      Materials:
        - ShaderName: EditorOutline.glsl
          ShaderProperties:
            []
        - ShaderName: GrayScale.glsl
          ShaderProperties:
            []
  - ID: 15369635725871284106
    Name: SceneSettings
    TransformComponent:
      Translation: [0, 0, 0]
      Rotation: [0, 0, 0]
      Scale: [1, 1, 1]
    SettingsComponent:
      AntiAliasingType: 1
      EditorCameraClearFlag: 1
      MSAASamples: 4
      ShowGizmos: true
    PostProcessingComponent:
      Materials:
        - ShaderName: EditorOutline.glsl
          ShaderProperties:
            []
  - ID: 5531910129310034619
    Name: Entity
    TransformComponent:
      Translation: [5.90144587, -3.92410779, -6.49440908]
      Rotation: [0, 0, 0]
      Scale: [1, 1, 1]
    ParticleSystemComponent:
      LauncherNum: 10
      SpawnAreaShape: 1
      SpawnPositionDistribution: 0
      SpawnAreaRadius: 2
      MaxVelocity: [0, 2, 0]
      MinVelocity: [0, 0.200000003, 0]
      MaxTotalLifetime: 5
      MinTotalLifetime: 3
      InitColor: [1, 1, 1, 1]
      InitSize: 1
      Pass1Name: Rising.glsl
      Pass1Props:
        []
      Pass2Name: ParticleSprite.glsl
      Pass2Props:
        - Type: 6
          Name: u_Color
          Value: [1, 1, 1]
        - Type: 15
          Name: u_Sprite
          Value: ../Assets/Textures/DontModify/DefaultParticle.png