Scene: Untitled
AntiAliasingType: 1
MSAASamples: 4
EditorCameraClearFlag: 0
ShowGizmos: 1
Entities:
  - ID: 15822961489110943461
    Name: Entity
    TransformComponent:
      Translation: [0, 0, 0]
      Rotation: [0, 0, 0]
      Scale: [1, 1, 1]
    ModelRendererComponent:
      ModelPath: ../Assets/Models/Box.obj
      Materials:
        - MaterialName: Material
          ShaderName: TexUnlit.glsl
          ShaderProperties:
            - Type: 6
              Name: u_Color
              Value: [1, 1, 1]
            - Type: 15
              Name: u_Custom
              Value: Default
            - Type: 15
              Name: u_Diffuse
              Value: ../Assets\Models\Firekeeper\Textures\c1400_hair_a.png
      Bones:
        []
  - ID: 15029275841816245841
    Name: Entity
    TransformComponent:
      Translation: [0, 0, 0]
      Rotation: [0, 0, 0]
      Scale: [1, 1, 1]
    ParticleSystemComponent:
      LauncherNum: 1860
      SpawnAreaShape: 2
      SpawnAreaRadius: 76
      MaxVelocity: [0, 2, 0]
      MinVelocity: [0, 0.200000003, 0]
      MaxTotalLifetime: 15
      MinTotalLifetime: 4
      InitColor: [1, 1, 1, 1]
      InitSize: 1
      Pass1Name: Rising.glsl
      Pass1Props:
        []
      Pass2Name: ParticleSprite.glsl
      Pass2Props:
        - Type: 6
          Name: u_Color
          Value: [0.864686489, 0.291082591, 0.291082591]
        - Type: 15
          Name: u_Sprite
          Value: ../Assets\Icons\DirLight.png