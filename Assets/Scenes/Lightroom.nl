Scene: Untitled
Entities:
  - ID: 356715956414432893
    Name: Cambridge
    TransformComponent:
      Translation: [2.25832009, 0.551451206, -6.8688941]
      Rotation: [-3.14159203, -0.0116237123, 3.14159226]
      Scale: [1, 1, 1]
    CameraComponent:
      FixedAspectRatio: false
      ClearColor: [0.192000002, 0.301999986, 0.474999994, 1]
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
  - ID: 126225866794501339
    Name: Entity
    TransformComponent:
      Translation: [-1.50262403, 1.25009191, 5.34430695]
      Rotation: [0, 0, 0]
      Scale: [1, 1, 1]
    CameraComponent:
      FixedAspectRatio: false
      ClearColor: [0.192000002, 0.301999986, 0.474999994, 1]
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
  - ID: 15246918702503044601
    Name: SceneSettings
    TransformComponent:
      Translation: [0, 0, 0]
      Rotation: [0, 0, 0]
      Scale: [1, 1, 1]
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
  - ID: 16313723394159567274
    Name: Entity
    TransformComponent:
      Translation: [0, 0, 0]
      Rotation: [0, 0, 0]
      Scale: [1, 1, 1]
    ModelRendererComponent:
      ModelPath: ../Assets/Models/Box.obj
      Materials:
        - MaterialName: Material
          ShaderName: Lit(MultipleLights).glsl
          ShaderProperties:
            - Type: 6
              Name: u_Color
              Value: [1, 1, 1]
            - Type: 1
              Name: u_AmbientStrength
              Value: 0.100000001
            - Type: 1
              Name: u_SpecularStrength
              Value: 0.100000001
      Bones:
        []
  - ID: 7931877917563106569
    Name: Entity
    TransformComponent:
      Translation: [-0.314063281, -0.133901909, 1.87836003]
      Rotation: [1.09098577, -0.471518755, -2.73589849]
      Scale: [0.99998343, 0.999998331, 0.999995589]
    LightComponent:
      LightType: 1
      Color: [1, 1, 1]
      Intensity: 1
      Attenuation: [1, 0.699999988, 1.79999995]
  - ID: 4418576179911461631
    Name: Entity
    TransformComponent:
      Translation: [1.67318439, 0, 0]
      Rotation: [0, 0, 0]
      Scale: [1, 1, 1]
    LightComponent:
      LightType: 1
      Color: [1, 1, 1]
      Intensity: 1
      Attenuation: [1, 0.699999988, 1.79999995]
  - ID: 12724278551602185364
    Name: Entity
    TransformComponent:
      Translation: [-0.21180445, 1.35760427, -0.418545723]
      Rotation: [0, 0, 0]
      Scale: [1, 1, 1]
    LightComponent:
      LightType: 1
      Color: [1, 1, 1]
      Intensity: 1
      Attenuation: [1, 0.699999988, 1.79999995]
  - ID: 15434772265538688380
    Name: Entity
    TransformComponent:
      Translation: [-2.13430429, -0.02648893, -0.0437014289]
      Rotation: [0, 0, 0]
      Scale: [1, 1, 1]
    LightComponent:
      LightType: 1
      Color: [1, 1, 1]
      Intensity: 1
      Attenuation: [1, 0.699999988, 1.79999995]
  - ID: 11997139433309534483
    Name: Entity
    TransformComponent:
      Translation: [0, 0, -1.86699224]
      Rotation: [0, 0, 0]
      Scale: [1, 1, 1]
    LightComponent:
      LightType: 1
      Color: [1, 1, 1]
      Intensity: 1
      Attenuation: [1, 0.699999988, 1.79999995]