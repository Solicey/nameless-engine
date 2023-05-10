Scene: Untitled
AntiAliasingType: 1
MSAASamples: 4
EditorCameraClearFlag: 0
ShowGizmos: 1
Entities:
  - ID: 2875367073407207992
    Name: Entity
    TransformComponent:
      Translation: [0, 1.69621491, 5.65779924]
      Rotation: [0, 0, 0]
      Scale: [1, 1, 1]
    CameraComponent:
      FixedAspectRatio: false
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
  - ID: 11701066803599960966
    Name: Entity
    TransformComponent:
      Translation: [0.0344234407, 0.0692740679, -4.35342455]
      Rotation: [0, 0, 0]
      Scale: [1, 1, 1]
    ParticleSystemComponent:
      LauncherNum: 200
      SpawnAreaShape: 1
      SpawnPositionDistribution: 1
      SpawnAreaRadius: 3
      MaxVelocity: [0, 1, 0]
      MinVelocity: [0, 0.200000003, 0]
      MaxTotalLifetime: 1.5
      MinTotalLifetime: 0.100000001
      InitColor: [1, 1, 1, 1]
      InitSize: 1
      Pass1Name: Fire.glsl
      Pass1Props:
        []
      Pass2Name: ParticleSprite.glsl
      Pass2Props:
        - Type: 6
          Name: u_Color
          Value: [0.214521468, 0.0736731961, 0.0141598331]
        - Type: 15
          Name: u_Sprite
          Value: ../Assets/Textures/DontModify/DefaultParticle.png