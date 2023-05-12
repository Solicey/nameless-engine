Scene: Untitled
AntiAliasingType: 1
MSAASamples: 4
EditorCameraClearFlag: 0
ShowGizmos: 1
Entities:
  - ID: 11701066803599960966
    Name: Fire
    TransformComponent:
      Translation: [0.0261335969, 0.262082994, 0]
      Rotation: [0, 0, 0]
      Scale: [1, 1, 1]
    ParticleSystemComponent:
      LauncherNum: 100
      SpawnAreaShape: 1
      SpawnPositionDistribution: 1
      SpawnAreaRadius: 1.29999995
      MaxVelocity: [0, 1, 0]
      MinVelocity: [0, 0.100000001, 0]
      MaxTotalLifetime: 2
      MinTotalLifetime: 0.200000003
      InitColor: [1, 1, 1, 1]
      InitSize: 1
      Pass1Name: Fire.glsl
      Pass1Props:
        []
      Pass2Name: ParticleSprite.glsl
      Pass2Props:
        - Type: 6
          Name: u_Color
          Value: [0.082508266, 0.0260387864, 0.00217843708]
        - Type: 15
          Name: u_Sprite
          Value: ../Assets/Textures/DontModify/DefaultParticle.png
  - ID: 16784744381934306163
    Name: Camera
    TransformComponent:
      Translation: [2.67311776e-07, 0.993012011, -3.7049787]
      Rotation: [3.10453176, 1.47106562e-07, -3.14159226]
      Scale: [0.999972343, 0.999980748, 1.00000167]
    CameraComponent:
      FixedAspectRatio: true
      ClearColor: [0.13861388, 0.13861388, 0.13861388, 1]
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
  - ID: 3293151802472727532
    Name: Ground
    TransformComponent:
      Translation: [0, 0, 0]
      Rotation: [0, 0, 0]
      Scale: [2, 2, 2]
    ModelRendererComponent:
      ModelPath: ../Assets/Models/Bonfire/Bonfire_Ground.fbx
      Materials:
        - MaterialName: o000100_00\N:\FDP\data\Model\obj\o00\o000100\tex\o000100_00_a.t
          ShaderName: TexLit(MultipleLights).glsl
          ShaderProperties:
            - Type: 6
              Name: u_Color
              Value: [1, 1, 1]
            - Type: 1
              Name: u_AmbientStrength
              Value: 2
            - Type: 1
              Name: u_SpecularStrength
              Value: 0
            - Type: 15
              Name: u_Diffuse
              Value: ../Assets\Models\Bonfire\o000100_00_a.png
            - Type: 15
              Name: u_Normals
              Value: ../Assets\Models\Bonfire\o000100_00_n.png
      Bones:
        []
  - ID: 15833219171818430500
    Name: Sword
    TransformComponent:
      Translation: [0.167404726, -0.102348566, 0]
      Rotation: [0, 0, 0.229568362]
      Scale: [2, 2, 2]
    ModelRendererComponent:
      ModelPath: ../Assets/Models/Bonfire/Bonfire_Sword.fbx
      Materials:
        - MaterialName: o000100_01\N:\FDP\data\Model\obj\o00\o000100\tex\o000100_01_a.t
          ShaderName: TexLit(MultipleLights).glsl
          ShaderProperties:
            - Type: 6
              Name: u_Color
              Value: [1, 1, 1]
            - Type: 1
              Name: u_AmbientStrength
              Value: 2
            - Type: 1
              Name: u_SpecularStrength
              Value: 0
            - Type: 15
              Name: u_Diffuse
              Value: ../Assets\Models\Bonfire\o000100_01_a.png
            - Type: 15
              Name: u_Normals
              Value: ../Assets\Models\Bonfire\o000100_01_n.png
      Bones:
        []
  - ID: 12802463022320250004
    Name: Light
    TransformComponent:
      Translation: [0.0616567135, 0.421330273, 0]
      Rotation: [0, 0, 0]
      Scale: [1, 1, 1]
    LightComponent:
      LightType: 1
      Color: [0.940594077, 0.521363854, 0.170734882]
      Intensity: 1.39999998
      Attenuation: [1, 0.699999988, 1.79999995]