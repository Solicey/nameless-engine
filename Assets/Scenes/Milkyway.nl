Scene: Untitled
AntiAliasingType: 1
MSAASamples: 4
EditorCameraClearFlag: 0
ShowGizmos: 1
Entities:
  - ID: 10556544261542424117
    Name: Milky Way
    TransformComponent:
      Translation: [7.38269377, 46.5330315, -268.059937]
      Rotation: [0.673996627, 0.555673003, 0.236581564]
      Scale: [1.00000024, 0.999999404, 0.999997973]
    ParticleSystemComponent:
      LauncherNum: 30000
      SpawnAreaShape: 1
      SpawnPositionDistribution: 0
      SpawnAreaRadius: 200
      MaxVelocity: [0, 2, 0]
      MinVelocity: [0, 0.200000003, 0]
      MaxTotalLifetime: 5
      MinTotalLifetime: 3
      InitColor: [1, 1, 1, 1]
      InitSize: 1
      Pass1Name: MilkywayPass1.glsl
      Pass1Props:
        - Type: 1
          Name: u_AlphaBias
          Value: 0.270000011
        - Type: 1
          Name: u_MaxHeight
          Value: 5
        - Type: 1
          Name: u_MaxRadius
          Value: 200
        - Type: 1
          Name: u_MinRadius
          Value: 50
        - Type: 1
          Name: u_GravitationalConst
          Value: 100
        - Type: 1
          Name: u_MaxCelestialRadius
          Value: 5
        - Type: 1
          Name: u_LuminanceThreshold
          Value: 0.270000011
        - Type: 15
          Name: u_Milkyway
          Value: ../Assets\Textures\MilkyWay.jpg
      Pass2Name: MilkywayPass2.glsl
      Pass2Props:
        - Type: 6
          Name: u_Color
          Value: [1, 1, 1]
        - Type: 15
          Name: u_Sprite
          Value: ../Assets/Textures/DontModify/DefaultParticle.png
  - ID: 5188054474898580972
    Name: Camera
    TransformComponent:
      Translation: [0, 0, 0]
      Rotation: [0, 0, 0]
      Scale: [1, 1, 1]
    CameraComponent:
      FixedAspectRatio: true
      ClearColor: [0, 0, 0, 1]
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