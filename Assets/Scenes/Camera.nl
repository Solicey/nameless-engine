Scene: Untitled
Entities:
  - ID: 18229387500907656058
    Name: 2
    TransformComponent:
      Translation: [5.95261049, -1.49011612e-08, 1.49011612e-08]
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
  - ID: 15550377673903496467
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
  - ID: 13624380140163365950
    Name: Entity
    TransformComponent:
      Translation: [0, 0, 0]
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
      ClearFlagType: 1
    PostProcessingComponent:
      Materials:
        - ShaderName: EditorOutline.glsl
          ShaderProperties:
            []
  - ID: 8920834719950260671
    Name: Entity
    TransformComponent:
      Translation: [0.770508885, 0.801162958, -7.66598463]
      Rotation: [0, 0, 0]
      Scale: [1, 1, 1]
    ModelRendererComponent:
      ModelPath: ../Assets/Models/Box.obj
      Materials:
        - MaterialName: Material
          ShaderName: Default.glsl
          ShaderProperties:
            []
      Bones:
        []