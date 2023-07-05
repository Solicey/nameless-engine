Scene: Untitled
Entities:
  - ID: 14588909198576569747
    Name: SceneSettings
    TransformComponent:
      Translation: [-1.19305611, 0.845535398, 8.72529507]
      Rotation: [0.335999519, 0, -0.302997082]
      Scale: [-3.04069686, -1.31704128, 2.81517315]
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
  - ID: 3392943982150419163
    Name: Entity
    TransformComponent:
      Translation: [0, 0, 0]
      Rotation: [0, 0, 0]
      Scale: [1, 1, 1]
    ModelRendererComponent:
      ModelPath: ../Assets/Models/breakfast_room/breakfast_room.obj
      Materials:
        - MaterialName: breakfast_room:Paint___White_Matt
          ShaderName: Default.glsl
          ShaderProperties:
            []
        - MaterialName: breakfast_room:Frosted_Glass
          ShaderName: Default.glsl
          ShaderProperties:
            []
        - MaterialName: breakfast_room:Ceramic_001
          ShaderName: Default.glsl
          ShaderProperties:
            []
        - MaterialName: breakfast_room:Material_002
          ShaderName: Default.glsl
          ShaderProperties:
            []
        - MaterialName: breakfast_room:Floor_Tiles
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
              Value: ../Assets/Models/breakfast_room/tiles.png
        - MaterialName: breakfast_room:Chrome
          ShaderName: Default.glsl
          ShaderProperties:
            []
        - MaterialName: breakfast_room:Black_Rubber
          ShaderName: Default.glsl
          ShaderProperties:
            []
        - MaterialName: breakfast_room:White_Plastic
          ShaderName: Default.glsl
          ShaderProperties:
            []
        - MaterialName: breakfast_room:Paint___Black_Satin
          ShaderName: Default.glsl
          ShaderProperties:
            []
        - MaterialName: breakfast_room:Gold_Paint
          ShaderName: Default.glsl
          ShaderProperties:
            []
        - MaterialName: breakfast_room:Ceramic
          ShaderName: Default.glsl
          ShaderProperties:
            []
        - MaterialName: breakfast_room:Paint___White_Gloss
          ShaderName: Default.glsl
          ShaderProperties:
            []
        - MaterialName: breakfast_room:Material_005
          ShaderName: Default.glsl
          ShaderProperties:
            []
        - MaterialName: breakfast_room:White_Marble
          ShaderName: Default.glsl
          ShaderProperties:
            []
        - MaterialName: breakfast_room:Artwork
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
              Value: ../Assets/Models/breakfast_room/picture3.jpg
      Bones:
        []