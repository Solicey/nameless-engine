Scene: Untitled
Entities:
  - ID: 2490664228888549516
    Name: SceneSettings
    TransformComponent:
      Translation: [-1.62891102, 4.42704916, 0.943425596]
      Rotation: [-0.912002861, -0.671837509, 0]
      Scale: [-0.142635554, 1.34262645, -0.925517261]
    SettingsComponent:
      AntiAliasingType: 1
      EditorCameraClearFlag: 0
      MSAASamples: 4
      ShowGizmos: true
      RenderMode: 0
    PostProcessingComponent:
      Materials:
        - ShaderName: EditorOutline.glsl
          ShaderProperties:
            []
  - ID: 4577811684396466908
    Name: Entity
    TransformComponent:
      Translation: [0, 0, -0.18382147]
      Rotation: [0, 0, 0]
      Scale: [1, 1, 1]
    ModelRendererComponent:
      ModelPath: ../Assets/Models/Box.fbx
      Materials:
        - MaterialName: DefaultMaterial
          ShaderName: TexLitReliefMapping.glsl
          ShaderProperties:
            - Type: 1
              Name: u_AmbientStrength
              Value: 0.100000001
            - Type: 1
              Name: u_SpecularStrength
              Value: 0
            - Type: 15
              Name: u_Diffuse
              Value: ../Assets\Textures\wood_texture.jpg
            - Type: 15
              Name: u_Specular
              Value: Default
            - Type: 15
              Name: u_Normals
              Value: ../Assets\Textures\toyBox_normal.png
            - Type: 1
              Name: u_IsNormalsCompressed
              Value: 0
            - Type: 15
              Name: u_ReliefMap
              Value: ../Assets\Textures\toyBox_depth.png
            - Type: 1
              Name: u_ReliefParam
              Value: 0.100000001
      Bones:
        []
  - ID: 1896171092369630761
    Name: Entity
    TransformComponent:
      Translation: [0.410878807, 1.87329519, -0.594684064]
      Rotation: [0, 0, 0]
      Scale: [1, 1, 1]
    LightComponent:
      LightType: 1
      Color: [1, 1, 1]
      Intensity: 3
      Attenuation: [1, 0.699999988, 1.79999995]
  - ID: 879538836753061882
    Name: Entity
    TransformComponent:
      Translation: [-4.28775948e-08, 1.81246305, -2.33175206]
      Rotation: [-3.14159226, 0, -3.14159226]
      Scale: [1, 1, 1]
    ModelRendererComponent:
      ModelPath: ../Assets/Models/Box.fbx
      Materials:
        - MaterialName: DefaultMaterial
          ShaderName: TexLitReliefMapping.glsl
          ShaderProperties:
            - Type: 1
              Name: u_AmbientStrength
              Value: 0
            - Type: 1
              Name: u_SpecularStrength
              Value: 0
            - Type: 15
              Name: u_Diffuse
              Value: ../Assets\Textures\wood_texture.jpg
            - Type: 15
              Name: u_Specular
              Value: Default
            - Type: 15
              Name: u_Normals
              Value: ../Assets\Textures\toyBox_normal.png
            - Type: 1
              Name: u_IsNormalsCompressed
              Value: 0
            - Type: 15
              Name: u_ReliefMap
              Value: ../Assets\Textures\toyBox_depth.png
            - Type: 1
              Name: u_ReliefParam
              Value: 0.0799999982
      Bones:
        []