Scene: Untitled
Entities:
  - ID: 2512236796659933535
    Name: Sphere
    TransformComponent:
      Translation: [-10.5939713, 7.17551756, -2.75045586]
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
  - ID: 7435904767540507848
    Name: TrackEnd
    TransformComponent:
      Translation: [-12.3673229, 0, 8.41509628]
      Rotation: [0, 0, 0]
      Scale: [1, 1, 1]
  - ID: 6196780381642991916
    Name: TrackBegin
    TransformComponent:
      Translation: [8.31302834, 0, 11.193327]
      Rotation: [0, 0, 0]
      Scale: [1, 1, 1]
  - ID: 12758287598739165521
    Name: CameraController
    TransformComponent:
      Translation: [0, 0, 0]
      Rotation: [0, 0, 0]
      Scale: [1, 1, 1]
    ScriptComponent:
      ClassName: HW2.CameraController
      ScriptFields:
        []
  - ID: 7976935081711616568
    Name: FixedCamera
    TransformComponent:
      Translation: [-22.1882839, 8.39671993, 16.9681034]
      Rotation: [-0.136586666, -0.63990581, -3.7152958e-07]
      Scale: [0.999999821, 1.00000012, 1]
    CameraComponent:
      FixedAspectRatio: false
      ClearColor: [0.192000002, 0.301999986, 0.474999994, 1]
      ProjectionType: 1
      OrthoSize: 10
      OrthoFar: 1
      OrthoNear: -1
      PerspFOV: 0.785398185
      PerspFar: 1000
      PerspNear: 0.00999999978
      ViewportWidth: 1280
      ViewportHeight: 720
  - ID: 1124694664969099408
    Name: Hand
    TransformComponent:
      Translation: [0.171829298, -2.85385323, -14.1492653]
      Rotation: [0.918735981, 2.98023224e-08, 1.5708009]
      Scale: [0.999952734, 1.00000906, 0.999993086]
    ModelRendererComponent:
      ModelPath: ../Assets/Models/Hand.fbx
      Materials:
        - MaterialName: 02 - Default
          ShaderName: DefaultWithBones.glsl
          ShaderProperties:
            []
      Bones:
        - Parent: "null"
          Child: metacarpals
        - Parent: metacarpals
          Child: thumb_proximal_phalange
        - Parent: thumb_intermediate_phalange
          Child: thumb_distal_phalange
        - Parent: thumb_proximal_phalange
          Child: thumb_intermediate_phalange
        - Parent: metacarpals
          Child: ring_proximal_phalange
        - Parent: metacarpals
          Child: middle_proximal_phalange
        - Parent: metacarpals
          Child: index_proximal_phalange
        - Parent: metacarpals
          Child: pinky_proximal_phalange
        - Parent: index_intermediate_phalange
          Child: index_distal_phalange
        - Parent: index_proximal_phalange
          Child: index_intermediate_phalange
        - Parent: middle_intermediate_phalange
          Child: middle_distal_phalange
        - Parent: middle_proximal_phalange
          Child: middle_intermediate_phalange
        - Parent: ring_intermediate_phalange
          Child: ring_distal_phalange
        - Parent: ring_proximal_phalange
          Child: ring_intermediate_phalange
        - Parent: pinky_intermediate_phalange
          Child: pinky_distal_phalange
        - Parent: pinky_proximal_phalange
          Child: pinky_intermediate_phalange
    ScriptComponent:
      ClassName: HW1.Hand
      ScriptFields:
        - Name: metacarpals_rot
          Type: Vector3
          Data: [0, 0, 0]
        - Name: index_proximal_rot
          Type: Vector3
          Data: [0, 0, 0]
        - Name: thumb_distal_rot
          Type: Vector3
          Data: [0, 0, 10]
  - ID: 7963179372585506637
    Name: FPSCamera
    TransformComponent:
      Translation: [-1.10769451, 2.16721606, 13.5970793]
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
  - ID: 13702378364795044523
    Name: TrackCamera
    TransformComponent:
      Translation: [-24.1449738, 2.97122717, -7.6469245]
      Rotation: [-3.14159203, -1.41775656, 3.14159226]
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
  - ID: 16990954705968512128
    Name: Box
    TransformComponent:
      Translation: [9.32452965, 0, -0.515009642]
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
  - ID: 2735088576646503286
    Name: Room
    TransformComponent:
      Translation: [0, 0, 0]
      Rotation: [0, 0, 0]
      Scale: [100, 100, 100]
    ModelRendererComponent:
      ModelPath: ../Assets/Models/Box.obj
      Materials:
        - MaterialName: Material
          ShaderName: Default.glsl
          ShaderProperties:
            []
      Bones:
        []