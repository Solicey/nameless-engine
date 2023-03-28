Scene: Untitled
Entities:
  - ID: 14281782599762842882
    Name: Cam2
    TransformComponent:
      Translation: [-2.48218632, -6.10351562e-05, 6.68563366]
      Rotation: [0, 0, 0]
      Scale: [1, 1, 1]
    CameraComponent:
      FixedAspectRatio: true
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
  - ID: 17290647773017276292
    Name: Entity
    TransformComponent:
      Translation: [-2.98023224e-08, 0, 0]
      Rotation: [0, 0, 0]
      Scale: [1, 1, 1]
    ModelRendererComponent:
      ModelPath: ../Assets/Models/Sphere.obj
      Materials:
        - MaterialName: None
          ShaderName: Default.glsl
          ShaderProperties:
            []
    ScriptComponent:
      ClassName: Test.T4est
  - ID: 5286011870214660209
    Name: Boxff
    TransformComponent:
      Translation: [-2.95244598, -1.03056812, -1.32929111]
      Rotation: [0, 0, 0]
      Scale: [0.330000043, 0.330000043, 0.330000043]
    ModelRendererComponent:
      ModelPath: ../Assets/Models/Box.obj
      Materials:
        - MaterialName: Material
          ShaderName: Unlit.glsl
          ShaderProperties:
            - Type: 6
              Name: u_Color
              Value: [1, 1, 1]
    ScriptComponent:
      ClassName: Test.Test
      ScriptFields:
        - Name: Speed
          Type: Float
          Data: 321
        - Name: Time
          Type: Float
          Data: 29
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
      OrthoFar: 230
      OrthoNear: -1
      PerspFOV: 0.785398185
      PerspFar: 1000
      PerspNear: 0.00999999978
      ViewportWidth: 1260
      ViewportHeight: 720