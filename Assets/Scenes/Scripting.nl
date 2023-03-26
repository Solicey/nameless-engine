Scene: Untitled
Entities:
  - ID: 5286011870214660209
    Name: Boxff
    TransformComponent:
      Translation: [-2.95244598, -1.04968023, -1.32929111]
      Rotation: [0, 0, 0]
      Scale: [0.330000043, 0.330000043, 0.330000043]
    ModelRendererComponent:
      ModelPath: ../Assets/Models/Box.obj
      Materials:
        - MaterialName: Material
          ShaderName: Default.glsl
          ShaderProperties:
            []
    ScriptComponent:
      ClassName: Test.Test
      ScriptFields:
        - Name: Speed
          Type: Float
          Data: 500
  - ID: 8131105510474574718
    Name: Camerafff
    TransformComponent:
      Translation: [-0.0130402446, 0, 6.80229235]
      Rotation: [0, 0, 0]
      Scale: [1, 1, 1]
    CameraComponent:
      FixedAspectRatio: true
      ClearColor: [0.192000002, 0.301999986, 0.474999994, 1]
      ProjectionType: 0
      OrthoSize: 10
      OrthoFar: 26
      OrthoNear: -1
      PerspFOV: 0.785398185
      PerspFar: 1000
      PerspNear: 0.00999999978
      ViewportWidth: 1280
      ViewportHeight: 720