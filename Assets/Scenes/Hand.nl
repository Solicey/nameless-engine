Scene: Untitled
Entities:
  - ID: 7976935081711616568
    Name: Cam
    TransformComponent:
      Translation: [-25.7987766, 17.0781498, 21.0965767]
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
      Translation: [0, 0, 0]
      Rotation: [0, 0, 0]
      Scale: [1, 1, 1]
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