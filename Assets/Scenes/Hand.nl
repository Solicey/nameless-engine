Scene: Untitled
Entities:
  - ID: 1124694664969099408
    Name: Hand
    TransformComponent:
      Translation: [0.171784759, 2.38418565e-07, -14.1492653]
      Rotation: [0.567681074, 1.16229057e-06, 1.57079768]
      Scale: [0.999957681, 1.00002062, 0.99998796]
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
      ClassName: Homework1.Hand
      ScriptFields:
        - Name: ring_distal_phalange
          Type: Int
          Data: 12
        - Name: index_distal_phalange
          Type: Int
          Data: 8
        - Name: ring_intermediate_phalange
          Type: Int
          Data: 13
        - Name: index_intermediate_phalange
          Type: Int
          Data: 9
        - Name: thumb_distal_phalange
          Type: Int
          Data: 2
        - Name: thumb_intermediate_phalange
          Type: Int
          Data: 3
        - Name: ring_proximal_phalange
          Type: Int
          Data: 4
        - Name: index_proximal_phalange
          Type: Int
          Data: 6
        - Name: thumb_proximal_phalange
          Type: Int
          Data: 1
        - Name: middle_proximal_phalange
          Type: Int
          Data: 5
        - Name: middle_intermediate_phalange
          Type: Int
          Data: 11
        - Name: middle_distal_phalange
          Type: Int
          Data: 10
        - Name: pinky_proximal_phalange
          Type: Int
          Data: 7
        - Name: pinky_intermediate_phalange
          Type: Int
          Data: 15
        - Name: pinky_distal_phalange
          Type: Int
          Data: 14
        - Name: anim_play_speed
          Type: Float
          Data: 15
  - ID: 7976935081711616568
    Name: Cam
    TransformComponent:
      Translation: [-26.1467247, 11.4713631, 13.4946222]
      Rotation: [-0.136586666, -0.63990581, -3.7152958e-07]
      Scale: [0.999999821, 1.00000012, 1]
    CameraComponent:
      FixedAspectRatio: true
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