Scene: Untitled
Entities:
  - ID: 8131105510474574718
    Name: Camerafff
    TransformComponent:
      Translation: [0.356283724, 0, 6.80229235]
      Rotation: [0, 0, 0]
      Scale: [1, 1, 1]
    CameraComponent:
      FixedAspectRatio: false
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
  - ID: 5286011870214660209
    Name: Boxff
    TransformComponent:
      Translation: [-2.94541907, -0.954915226, -1.36292136]
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
              Value: [1, 0.293729365, 0.293729365]
      Bones:
        []
    ScriptComponent:
      ClassName: Test.Test
      ScriptFields:
        - Name: Speed
          Type: Float
          Data: 273
        - Name: Time
          Type: Float
          Data: 29
  - ID: 17290647773017276292
    Name: Entity
    TransformComponent:
      Translation: [0.000916182995, 0, 0.0265585184]
      Rotation: [0, 0, 0]
      Scale: [1, 1, 1]
    ModelRendererComponent:
      ModelPath: ../Assets/Models/Sphere.obj
      Materials:
        - MaterialName: None
          ShaderName: Default.glsl
          ShaderProperties:
            []
      Bones:
        []
    ScriptComponent:
      ClassName: "Test.Testjj "
  - ID: 14281782599762842882
    Name: Cam2
    TransformComponent:
      Translation: [-2.48218632, -6.10351562e-05, 6.65038443]
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