Scene: Untitled
Entities:
  - ID: 747965960863692057
    Name: SceneSettings
    TransformComponent:
      Translation: [0, 0, 0]
      Rotation: [0, 0, 0]
      Scale: [1, 1, 1]
    SettingsComponent:
      AntiAliasingType: 0
      EditorCameraClearFlag: 0
      MSAASamples: 4
      ShowGizmos: true
      RenderMode: 1
    PostProcessingComponent:
      Materials:
        - ShaderName: DeferredShading.glsl
          ShaderProperties:
            []
        - ShaderName: EditorOutline.glsl
          ShaderProperties:
            []
  - ID: 4002612728789966827
    Name: Entity
    TransformComponent:
      Translation: [-1.76037645, 0.842040718, 0.981138706]
      Rotation: [0, 0, 0]
      Scale: [1, 1, 1]
    ModelRendererComponent:
      ModelPath: ../Assets/Models/Box.obj
      Materials:
        - MaterialName: Material
          ShaderName: TexLit(Deferred).glsl
          ShaderProperties:
            - Type: 6
              Name: u_Color
              Value: [1, 1, 1]
            - Type: 1
              Name: u_SpecularStrength
              Value: 4
            - Type: 15
              Name: u_Diffuse
              Value: ../Assets\Models\vampire\test\textures\Vampire_diffuse.png
            - Type: 15
              Name: u_Specular
              Value: ../Assets\Models\vampire\test\textures\Vampire_specular.png
            - Type: 15
              Name: u_Normals
              Value: ../Assets\Models\vampire\test\textures\Vampire_normal.png
      Bones:
        []