Scene: Untitled
Entities:
  - ID: 8340806378877493287
    Name: Entity
    TransformComponent:
      Translation: [-1.83225274, 0, -0.983570874]
      Rotation: [0, 0, 0]
      Scale: [1, 1, 1]
    LightComponent:
      LightType: 1
      Color: [1, 1, 1]
      Intensity: 2
      Attenuation: [1, 0.699999988, 1.79999995]
  - ID: 17410514207797180759
    Name: Entity
    TransformComponent:
      Translation: [0, 0, 0]
      Rotation: [0, 0, 0]
      Scale: [1, 1, 1]
    ModelRendererComponent:
      ModelPath: ../Assets/Models/Box.fbx
      Materials:
        - MaterialName: DefaultMaterial
          ShaderName: TexLitParallaxMapping.glsl
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
              Name: u_HeightMap
              Value: ../Assets\Textures\toyBox_depth.png
            - Type: 1
              Name: u_ParallaxParam
              Value: 0.0500000007
      Bones:
        []
  - ID: 17405362916391556586
    Name: SceneSettings
    TransformComponent:
      Translation: [-3.62060976, 2.06616306, 0.587269902]
      Rotation: [-0.513000965, -1.40999496, -9.30686213e-08]
      Scale: [0, 0, 0]
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