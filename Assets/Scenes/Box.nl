Scene: Untitled
Entities:
  - ID: 16241618240402484313
    Name: Entity
    TransformComponent:
      Translation: [0, 0, 1.38888335]
      Rotation: [0, 0, 0]
      Scale: [1, 1, 1]
    LightComponent:
      LightType: 0
      Color: [1, 1, 1]
      Intensity: 1
      Attenuation: [1, 0.699999988, 1.79999995]
  - ID: 251931277435229708
    Name: SceneSettings
    TransformComponent:
      Translation: [0.724750996, -0.0941501856, 6.04037809]
      Rotation: [0.135000169, 0, -0.0419987477]
      Scale: [0.416056633, -1.0927788, -1.30538499]
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
  - ID: 10164215708358918445
    Name: Entity
    TransformComponent:
      Translation: [0.533386171, 0.307433963, -3.77377748]
      Rotation: [0, 0, 0]
      Scale: [1, 1, 1]
    ModelRendererComponent:
      ModelPath: ../Assets/Models/Box.fbx
      Materials:
        - MaterialName: DefaultMaterial
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
              Value: ../Assets\Textures\wood_texture.jpg
      Bones:
        []