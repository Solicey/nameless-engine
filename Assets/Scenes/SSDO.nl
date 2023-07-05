Scene: Untitled
Entities:
  - ID: 4608802449465100154
    Name: Entity
    TransformComponent:
      Translation: [0, 0, 0]
      Rotation: [0, 0, 0]
      Scale: [1, 1, 1]
  - ID: 17955367600477968809
    Name: Entity
    TransformComponent:
      Translation: [-4.39532995, 4.5114274, 0.794258416]
      Rotation: [1.19676411, -0.160313055, -0.386253238]
      Scale: [0.999997616, 1.00000143, 1.00000119]
    LightComponent:
      LightType: 0
      Color: [1, 1, 1]
      Intensity: 0.5
      Attenuation: [6.80000019, -2.0999999, 0.699999988]
  - ID: 7263971091382091159
    Name: SceneSettings
    TransformComponent:
      Translation: [6.44991207, 5.92834568, -3.3024044]
      Rotation: [6.64641142, 0, -1.94053233]
      Scale: [-2.30641866, 2.35893655, 0.0911968499]
    SettingsComponent:
      AntiAliasingType: 0
      EditorCameraClearFlag: 1
      MSAASamples: 4
      ShowGizmos: true
      RenderMode: 1
    PostProcessingComponent:
      Materials:
        - ShaderName: SSDO.glsl
          ShaderProperties:
            - Type: 10
              Name: u_KernelSize
              Value: 128
            - Type: 1
              Name: u_Radius
              Value: 3.0999999
            - Type: 1
              Name: u_Threshold
              Value: 0.00999999978
            - Type: 6
              Name: u_AmbientColor
              Value: [1, 1, 1]
        - ShaderName: Blur.glsl
          ShaderProperties:
            []
        - ShaderName: DeferredShading(SSDO).glsl
          ShaderProperties:
            []
  - ID: 10035270213595943175
    Name: Entity
    TransformComponent:
      Translation: [-2.50136495, 1.11159003, 1.1920929e-06]
      Rotation: [-1.57079566, 4.80562449e-07, 0.028215589]
      Scale: [10, 9.99785614, 9.99792004]
    ModelRendererComponent:
      ModelPath: ../Assets/Models/BiggerPlane.fbx
      Materials:
        - MaterialName: DefaultMaterial
          ShaderName: Default.glsl
          ShaderProperties:
            []
      Bones:
        []
  - ID: 9021933770027055054
    Name: Entity
    TransformComponent:
      Translation: [-6.13018751, 1.13416171, 1.10252059]
      Rotation: [0, 0, -0.969460309]
      Scale: [0.999999881, 0.999999881, 4.30000019]
    ModelRendererComponent:
      ModelPath: ../Assets/Models/Box.fbx
      Materials:
        - MaterialName: DefaultMaterial
          ShaderName: Default.glsl
          ShaderProperties:
            []
      Bones:
        []
  - ID: 3472773392530139733
    Name: Entity
    TransformComponent:
      Translation: [0.475124121, 1.86106169, 0.0334601998]
      Rotation: [0, 0, 0]
      Scale: [1, 1.20000005, 1]
    ModelRendererComponent:
      ModelPath: ../Assets/Models/Sphere.fbx
      Materials:
        - MaterialName: DefaultMaterial
          ShaderName: Default.glsl
          ShaderProperties:
            []
      Bones:
        []