#include <string>
#include <vector>

namespace Operations {

struct B {
    bool _is_obsolete{false};
    std::string _library_name;
    std::string _process_name;
    std::string _labels;
    std::string _deformation_type;
    std::string _speed_column_name;
    std::string _trigger;
    std::string _sql_query_formula;
};

struct RootHeat : B {

    const int type_id = 3;
    const int parent_type_id = 1;

    std::string name;

    RootHeat()
    {
        _is_obsolete = false;
        _library_name = "Heating operation";
        _process_name = "{} Heating";
        _labels = "Heating operation name/number:";
        _deformation_type = "";
        _speed_column_name = "";
        _trigger = "initialize";
        _sql_query_formula = "name VARCHAR(255) DEFAULT NULL";
    }
};

struct RootHeatFurnace : B {

    const int type_id = 10;
    const int parent_type_id = 3;

    int16_t furnace_class_id;

    // Foreign key helper methods (stubs)
    static std::string furnace_class(int16_t furnace_class_id) {
        // TODO: Call corresponding class or service to resolve 'furnace_class_name' by 'furnace_class_id'.
        return {};
    }

    RootHeatFurnace()
        : furnace_class_id(0)
    {
        _is_obsolete = false;
        _library_name = "Furnace class";
        _process_name = "Furnace class: {}";
        _labels = "Furnace class:";
        _deformation_type = "";
        _speed_column_name = "";
        _trigger = "accumulate";
        _sql_query_formula = "furnace_class_id SMALLINT NOT NULL DEFAULT 1, "
                             "FOREIGN KEY (furnace_class_id) REFERENCES furnace_class(furnace_class_id) ON DELETE SET DEFAULT";
    }
};

struct RootHeatHeatingCurve : B {
    const int type_id = 11;
    const int parent_type_id = 3;

    RootHeatHeatingCurve()
    {
        _is_obsolete = false;
        _library_name = "Heating curve";
        _process_name = "Heating curve";
        _labels = "";
        _deformation_type = "";
        _speed_column_name = "";
        _trigger = "keep";
        _sql_query_formula = "";
    }
};

struct RootHeatHeatingCurveFirstPoint : B {
    const int type_id = 62;
    const int parent_type_id = 11;

    std::string temperature;

    RootHeatHeatingCurveFirstPoint()
    {
        _is_obsolete = false;
        _library_name = "Temperature at 0 min";
        _process_name = "{} °C, 0 min";
        _labels = "Temperature point [°C]:";
        _deformation_type = "";
        _speed_column_name = "";
        _trigger = "keep";
        _sql_query_formula = "temperature NUMERIC(9, 3) DEFAULT NULL,";
    }
};

struct RootHeatHeatingCurveNextPoints : B {

    const int type_id = 23;
    const int parent_type_id = 11;

    std::string temperature;
    std::string duration;

    RootHeatHeatingCurveNextPoints()
    {
        _is_obsolete = false;
        _library_name = "Temperature, Duration";
        _process_name = "{} °C, {} min";
        _labels = "Temperature point [°C]:|Duration since previous [min]:";
        _deformation_type = "";
        _speed_column_name = "";
        _trigger = "keep";
        _sql_query_formula = "temperature NUMERIC(9, 3) DEFAULT NULL, "
                             "duration NUMERIC(12, 6) DEFAULT NULL,";
    }
};

struct RootForging : B {

    const int type_id = 4;
    const int parent_type_id = 1;


    std::string name;

    RootForging()
        : name("Operation 1")
    {
        _is_obsolete = false;
        _library_name = "Forging operation";
        _process_name = "{} Forging";
        _labels = "Forging operation name/number:";
        _deformation_type = "";
        _speed_column_name = "";
        _trigger = "initialize";
        _sql_query_formula = "name VARCHAR(255) DEFAULT NULL";
    }
};

struct Press : B {

    const int type_id = 26;
    const int parent_type_id = 24;

    std::size_t press_id;

    // Foreign key helper methods (stubs)
    static std::string press(std::size_t press_id) {
        // TODO: Call corresponding class or service to resolve 'name' by 'press_id'.
        return {};
    }

    Press()
        : press_id(0)
    {
         _is_obsolete = false;
        _library_name = "Press";
        _process_name = "Press: {}";
        _labels = "Press name:";
        _deformation_type = "";
        _speed_column_name = "";
        _trigger = "accumulate";
        _sql_query_formula = "press_id SMALLINT NOT NULL DEFAULT 1, "
                             "FOREIGN KEY (press_id) REFERENCES press(press_id) ON DELETE SET DEFAULT";
    }
};

struct Speed : B {

    const int type_id = 9;
    const int parent_type_id = 24;

    std::string speed_upsetting;
    std::string speed_prolongation;
    std::string speed_full_die;
    
    struct SpeedItem {
        double speed_value;
        std::string _library_name;
        std::string _process_name;
        std::string _labels;
        std::string _speed_column_name;
        std::string _sql_query_formula;
    };

    std::vector<SpeedItem> speed = {
        {
           0,
           "Upsetting: V mm/s",
           "Upsetting: {} mm/s",
           "Upsetting speed [mm/s]",
           "",
           "speed_upsetting NUMERIC(9, 3) DEFAULT NULL"
        },
        {
           0,
           "Prolongation: V mm/s",
           "Prolongation: {} mm/s",
           "Prolongation speed [mm/s]",
           "",
           "speed_prolongation NUMERIC(9, 3) DEFAULT NULL"
        },
        {
           0,
           "Full die: V mm/s",
           "Full die: {} mm/s",
           "Full die speed [mm/s]",
           "",
           "speed_full_die NUMERIC(9, 3) DEFAULT NULL"
        },
    };
};

struct Feed : B {

    const int type_id = 18;
    const int parent_type_id = 12;

    std::string feed_direction_id;
    std::string feed_first;
    std::string feed_middle;
    std::string feed_last;

    // Foreign key helper methods (stubs)
    static std::string feed_direction(std::size_t feed_direction_id) {
        // TODO: Call corresponding class or service to resolve 'feed_direction_name' by 'feed_direction_id'.
        return {};
    }

    Feed()
    {
        _is_obsolete = false;
        _library_name = "Direction, Feed mm, next - Feed mm, last - Feed mm";
        _process_name = "{}, {} mm, next - {} mm, last - {} mm";
        _labels = "Select feed direction:|First feed length [mm]:|Next feed length [mm]:|Last feed length [mm]";
        _deformation_type = "";
        _speed_column_name = "";
        _trigger = "accumulate";
        _sql_query_formula = "feed_direction_id SMALLINT DEFAULT 2, "
                             "feed_first NUMERIC(9, 3) DEFAULT NULL, "
                             "feed_middle NUMERIC(9, 3) DEFAULT NULL, "
                             "feed_last NUMERIC(9, 3) DEFAULT NULL, "
                             "FOREIGN KEY (feed_direction_id) REFERENCES feed_direction(feed_direction_id)  ON DELETE SET DEFAULT";
    }
};

struct Upsetting : B {

    const int type_id = 37;
    const int parent_type_id = 36;

    Upsetting()
    {
        _is_obsolete = false;
        _library_name = "upsetting";
        _process_name = "upsetting";
        _labels = "";
        _deformation_type = "";
        _speed_column_name = "";
        _trigger = "keep";
        _sql_query_formula = "";
    }
};

struct UpsettingSingleOrTripleRotation : B {

    const int type_id = 91;
    const int parent_type_id = 37;

    std::string rotation;
    std::string height;

    UpsettingSingleOrTripleRotation()
    : rotation("0")
    , height("0")
    {
        _is_obsolete = false;
        _library_name = "↓ (∠°) H";
        _process_name = "↓ ({}°) {}";
        _labels = "Rotate before upsetting [°]:|↓ Height of upsetting [mm]:";
        _deformation_type = "upsetting";
        _speed_column_name = "speed_upsetting";
        _trigger = "keep";
        _sql_query_formula = "rotation NUMERIC(6, 3) DEFAULT NULL, "
                             "height NUMERIC(9, 3) DEFAULT NULL";
    }
};

struct UpsettingSingleOrTriple : B {

    const int type_id = 42;
    const int parent_type_id = 37;

    std::string height;

    UpsettingSingleOrTriple()
    : height("0")
    {
        _is_obsolete = true;
        _library_name = "↓ H";
        _process_name = "↓ {}";
        _labels = "↓ Height of upsetting [mm]:";
        _deformation_type = "upsetting";
        _speed_column_name = "speed_upsetting";
        _trigger = "keep";
        _sql_query_formula = "height NUMERIC(9, 3) DEFAULT NULL";
    }
};

struct UpsettingSingle : B {

    const int type_id = 41;
    const int parent_type_id = 37;

    std::string height;

    UpsettingSingle()
    {
        _is_obsolete = true;
        _library_name = "↓ H, single stroke strictly";
        _process_name = "↓ {}, single stroke strictly";
        _labels = "↓ Height of upsetting [mm]:";
        _deformation_type = "upsetting";
        _speed_column_name = "speed_upsetting";
        _trigger = "keep";
        _sql_query_formula = "height NUMERIC(9, 3) DEFAULT NULL";
    }
};

struct UpsettingTriple : B {

    const int type_id = 43;
    const int parent_type_id = 37;

    std::string height;

    UpsettingTriple()
    {
        _is_obsolete = true;
        _library_name = "↓ H, three strokes";
        _process_name = "↓ {}, three strokes";
        _labels = "↓ Height of upsetting [mm]:";
        _deformation_type = "upsetting";
        _speed_column_name = "speed_upsetting";
        _trigger = "keep";
        _sql_query_formula = "height NUMERIC(9, 3) DEFAULT NULL";
    }
};

struct UpsettingSingleRotate : B {

    const int type_id = 93;
    const int parent_type_id = 37;


    std::string rotation;
    std::string height;

    UpsettingSingleRotate()
    {
        _is_obsolete = false;
        _library_name = "↓ (∠°) H, single stroke";
        _process_name = "↓ ({}°) {}, single stroke";
        _labels = "Rotate before upsetting [°]:|↓ Height of upsetting [mm]:";
        _deformation_type = "upsetting";
        _speed_column_name = "speed_upsetting";
        _trigger = "keep";
        _sql_query_formula = "rotation NUMERIC(6, 3) DEFAULT NULL, height NUMERIC(9, 3) DEFAULT NULL";
    }
};

struct UpsettingTripleRotate : B {

    const int type_id = 94;
    const int parent_type_id = 37;


    std::string rotation;
    std::string height;

    UpsettingTripleRotate()
    {
        _is_obsolete = false;
        _library_name = "↓ (∠°) H, three strokes (middle then sides)";
        _process_name = "↓ ({}°) {}, three strokes (middle then sides)";
        _labels = "Rotate before upsetting [°]:|↓ Height of upsetting [mm]:";
        _deformation_type = "upsetting";
        _speed_column_name = "speed_upsetting";
        _trigger = "keep";
        _sql_query_formula = "rotation NUMERIC(6, 3) DEFAULT NULL, "
                             "height NUMERIC(9, 3) DEFAULT NULL";
    }
};

struct UpsettingRadialFeed : B {

    const int type_id = 97;
    const int parent_type_id = 37;

    std::string rotation;
    std::string height;
    std::string radial_feed;

    UpsettingRadialFeed()
    {
        _is_obsolete = false;
        _library_name = "↓ (∠°) H, radial feed";
        _process_name = "↓ ({}°) {}, radial feed {} mm";
        _labels = "Rotate around press axis [°]:|↓ Height of upsetting [mm]:|Radial feed [mm]:";
        _deformation_type = "radial_prolongation";
        _speed_column_name = "speed_prolongation";
        _trigger = "keep";
        _sql_query_formula = "rotation NUMERIC(6, 3) DEFAULT NULL, "
                             "height NUMERIC(9, 3) DEFAULT NULL, "
                             "radial_feed NUMERIC(9, 3) DEFAULT NULL";
    }
};

struct UpsettingTailFlattening : B {

    const int type_id = 44;
    const int parent_type_id = 37;

    std::string stroke;

    UpsettingTailFlattening()
    {
        _is_obsolete = true;
        _library_name = "↓ ΔH, tail flattening";
        _process_name = "↓ Δ{}, tail flattening";
        _labels = "Stroke distance of tail flattening [mm]:";
        _deformation_type = "upsetting";
        _speed_column_name = "speed_upsetting";
        _trigger = "keep";
        _sql_query_formula = "stroke NUMERIC(9, 3) DEFAULT NULL";
    }
};

struct UpsettingTailFlatteningWithRotation : B {

    const int type_id = 92;
    const int parent_type_id = 37;

    std::string rotation;
    std::string stroke;

    UpsettingTailFlatteningWithRotation()
    {
        _is_obsolete = false;
        _library_name = "↓ (∠°) ΔH, tail flattening";
        _process_name = "↓ ({}°) Δ{}, tail flattening";
        _labels = "Rotate before tail flattening [°]:|Stroke distance of tail flattening [mm]:";
        _deformation_type = "upsetting";
        _speed_column_name = "speed_upsetting";
        _trigger = "keep";
        _sql_query_formula = "rotation NUMERIC(6, 3) DEFAULT NULL, "
                             "stroke NUMERIC(9, 3) DEFAULT NULL";
    }
};

struct UpsettingTailChamfering : B {

    const int type_id = 45;
    const int parent_type_id = 37;

    UpsettingTailChamfering()
    {
        _is_obsolete = true;
        _library_name = "Tail chamfering";
        _process_name = "Tail chamfering";
        _labels = "";
        _deformation_type = "upsetting";
        _speed_column_name = "speed_upsetting";
        _trigger = "keep";
        _sql_query_formula = "";
    }
};

struct UpsettingTailChamferingRotation : B {

    const int type_id = 100;
    const int parent_type_id = 37;

    std::string rotation;

    UpsettingTailChamferingRotation()
    {
        _is_obsolete = false;
        _library_name = "↓ (∠°) Tail chamfering";
        _process_name = "↓ ({}°) Tail chamfering";
        _labels = "Rotate before tail chamfering [°]:";
        _deformation_type = "upsetting";
        _speed_column_name = "speed_upsetting";
        _trigger = "keep";
        _sql_query_formula = "rotation NUMERIC(6, 3) DEFAULT NULL";
    }
};

struct AxialProlongation : B {

    const int type_id = 38;
    const int parent_type_id = 36;

    AxialProlongation()
    {
        _is_obsolete = false;
        _library_name = "axial prolongation";
        _process_name = "axial prolongation";
        _labels = "";
        _deformation_type = "";
        _speed_column_name = "";
        _trigger = "keep";
        _sql_query_formula = "";
    }
};

struct AxialProlongationSimple : B {

    const int type_id = 46;
    const int parent_type_id = 38;


    std::string rotation;
    std::string height;

    AxialProlongationSimple()
    {
        _is_obsolete = false;
        _library_name = "→ (∠°) H";
        _process_name = "→ ({}°) {}";
        _labels = "Rotation before prolongation [°]:|Height of prolongation [mm]:";
        _deformation_type = "axial_prolongation";
        _speed_column_name = "speed_prolongation";
        _trigger = "keep";
        _sql_query_formula = "rotation NUMERIC(6, 3) DEFAULT NULL, "
                             "height NUMERIC(9, 3) DEFAULT NULL";
    }
};

struct AxialProlongationNumOfBites : B {

    const int type_id = 83;
    const int parent_type_id = 38;


    std::string rotation;
    std::string height;
    std::string num_of_bites;

    AxialProlongationNumOfBites()
    {
        _is_obsolete = false;
        _library_name = "→ (∠°) H(n)";
        _process_name = "→ ({}°) {} ({})";
        _labels = "Rotation before prolongation [°]:|Height of prolongation [mm]:|Number of bites []:";
        _deformation_type = "axial_prolongation";
        _speed_column_name = "speed_prolongation";
        _trigger = "keep";
        _sql_query_formula = "rotation NUMERIC(6, 3) DEFAULT NULL, "
                             "height NUMERIC(9, 3) DEFAULT NULL, "
                             "num_of_bites SMALLINT DEFAULT NULL";
    }
};

struct AxialProlongationNumOfBitesSkipBites : B {

    const int type_id = 90;
    const int parent_type_id = 38;


    std::string rotation;
    std::string height;
    std::string num_of_bites;
    std::string skip_bites;

    AxialProlongationNumOfBitesSkipBites()
    {
        _is_obsolete = false;
        _library_name = "→ (∠°) H(n) skip bites";
        _process_name = "→ ({}°) {} ({}) skip bites {}";
        _labels = "Rotation before prolongation [°]:|Height of prolongation [mm]:|Number of bites []:|Skip bites ## (comma separated):";
        _deformation_type = "axial_prolongation";
        _speed_column_name = "speed_prolongation";
        _trigger = "keep";
        _sql_query_formula = "rotation NUMERIC(6, 3) DEFAULT NULL, "
                             "height NUMERIC(9, 3) DEFAULT NULL, "
                             "num_of_bites SMALLINT DEFAULT NULL, "
                             "skip_bites VARCHAR(255) DEFAULT NULL";
    }
};

struct AxialProlongationFindDiagonal : B {

    const int type_id = 47;
    const int parent_type_id = 38;

    std::string rotation;
    std::string height;

    AxialProlongationFindDiagonal()
    {
        _is_obsolete = true;
        _library_name = "(∠°) H, start from diagonal (adjust rotation angle)";
        _process_name = "({}°) {}, start from diagonal (adjust rotation angle)";
        _labels = "Rotation before prolongation [°]:|Height of prolongation [mm]:";
        _deformation_type = "axial_prolongation";
        _speed_column_name = "speed_prolongation";
        _trigger = "keep";
        _sql_query_formula = "rotation NUMERIC(6, 3) DEFAULT NULL, "
                             "height NUMERIC(9, 3) DEFAULT NULL";
    }
};

struct AxialProlongationStartFromHeight : B {

    const int type_id = 48;
    const int parent_type_id = 38;

    std::string height;

    AxialProlongationStartFromHeight()
    {
        _is_obsolete = true;
        _library_name = "(0...90°) H, rotate and start from highest side of rectangular section";
        _process_name = "(0...90°) {}, rotate and start from highest side of rectangular section";
        _labels = "Height of prolongation [mm]:";
        _deformation_type = "axial_prolongation";
        _speed_column_name = "speed_prolongation";
        _trigger = "keep";
        _sql_query_formula = "height NUMERIC(9, 3) DEFAULT NULL";
    }
};

struct SpiralProlongation : B {

    const int type_id = 99;
    const int parent_type_id = 36;

    SpiralProlongation()
    {
        _is_obsolete = false;
        _library_name = "spiral prolongation";
        _process_name = "spiral prolongation";
        _labels = "";
        _deformation_type = "";
        _speed_column_name = "";
        _trigger = "keep";
        _sql_query_formula = "";
    }
};

struct SpiralProlongationRoundingSpiral1Rotation1Feed : B {

    const int type_id = 50;
    const int parent_type_id = 99;


    std::string diameter;
    std::string rotation_per_bite;
    std::string feed;

    SpiralProlongationRoundingSpiral1Rotation1Feed()
    {
        _is_obsolete = false;
        _library_name = "⌀D, spiral rounding 1 rotation per 1 feed";
        _process_name = "⌀{}, spiral rounding 1 rotation {}° per 1 feed {} mm";
        _labels = "Final diameter [mm]:|Rotation [°]:|Feed [mm]:";
        _deformation_type = "axial_prolongation";
        _speed_column_name = "speed_prolongation";
        _trigger = "keep";
        _sql_query_formula = "diameter NUMERIC(9, 3) DEFAULT NULL, "
                             "rotation_per_bite NUMERIC(6, 3) DEFAULT NULL, "
                             "feed NUMERIC(9, 3) DEFAULT NULL";
    }
};

struct SpiralProlongationRoundingSpiral3Rotation1Feed : B {

    const int type_id = 51;
    const int parent_type_id = 99;

    std::string diameter;
    std::string rotation_per_bite;
    std::string feed;

    SpiralProlongationRoundingSpiral3Rotation1Feed()
    {
        _is_obsolete = false;
        _library_name = "⌀D, spiral rounding 3 rotations per 1 feed";
        _process_name = "⌀{}, spiral rounding 3 rotations {}° per 1 feed {} mm";
        _labels = "Final diameter [mm]:|Rotation [°]:|Feed [mm]:";
        _deformation_type = "axial_prolongation";
        _speed_column_name = "speed_prolongation";
        _trigger = "keep";
        _sql_query_formula = "diameter NUMERIC(9, 3) DEFAULT NULL, "
                             "rotation_per_bite NUMERIC(6, 3) DEFAULT NULL, "
                             "feed NUMERIC(9, 3) DEFAULT NULL";
    }
};

struct SpiralProlongationRoundingStraight : B {

    const int type_id = 49;
    const int parent_type_id = 99;


    std::string rotation;
    std::string diameter;

    SpiralProlongationRoundingStraight()
    {
        _is_obsolete = true;
        _library_name = "(∠°) ⌀D, straight feed without bite rotations (not spiral)";
        _process_name = "({}°) ⌀{}, straight feed without bite rotations (not spiral)";
        _labels = "Rotation before prolongation [°]:|Diameter of round section [mm]:";
        _deformation_type = "axial_prolongation";
        _speed_column_name = "speed_prolongation";
        _trigger = "keep";
        _sql_query_formula = "diameter NUMERIC(9, 3) DEFAULT NULL, "
                             "rotation NUMERIC(6, 3) DEFAULT NULL";
    }
};

struct RadialProlongation : B {

    const int type_id = 35;
    const int parent_type_id = 36;

    RadialProlongation()
    {
        _is_obsolete = false;
        _library_name = "radial prolongation";
        _process_name = "radial prolongation";
        _labels = "";
        _deformation_type = "";
        _speed_column_name = "";
        _trigger = "keep";
        _sql_query_formula = "";
    }
};

struct RadialProlongationRadialInitialRotation : B {

    const int type_id = 98;
    const int parent_type_id = 35;


    std::string rotation_1_manipulator;
    std::string rotation_3_manipulator;
    std::string rotation_4_operator;

    RadialProlongationRadialInitialRotation()
    {
        _is_obsolete = false;
        _library_name = "initial rotations (manipulator ∠°, operator 90°, manipulator ∠°, operator ∠°)";
        _process_name = "initial rotations (manipulator {}°, operator 90°, manipulator {}°, operator {}°)";
        _labels = "1st rotation around manipulator axis [°]:|3rd rotation around manipulator axis [°]:|4th rotation around operator view axis [°]:";
        _deformation_type = "";
        _speed_column_name = "";
        _trigger = "keep";
        _sql_query_formula = "rotation_1_manipulator NUMERIC(6, 3) DEFAULT NULL, "
                             "rotation_3_manipulator NUMERIC(6, 3) DEFAULT NULL, "
                             "rotation_4_operator NUMERIC(6, 3) DEFAULT NULL";
    }
};

struct RadialProlongationRadialRotationManipulatorAndFeed : B {

    const int type_id = 95;
    const int parent_type_id = 35;

    std::string rotation_manipulator;
    std::string height;
    std::string radial_feed;

    RadialProlongationRadialRotationManipulatorAndFeed()
    {
        _is_obsolete = false;
        _library_name = "↓ → (∠°), H, feed";
        _process_name = "↓ → ({}°), {} mm, feed {} mm";
        _labels = "Rotation around manipulator axis [°]:|Height [mm]:|Feed [mm]:";
        _deformation_type = "radial_prolongation";
        _speed_column_name = "speed_prolongation";
        _trigger = "keep";
        _sql_query_formula = " rotation_manipulator NUMERIC(6, 3) DEFAULT NULL, "
                             "height NUMERIC(9, 3) DEFAULT NULL, "
                             "radial_feed NUMERIC(9, 3) DEFAULT NULL";
    }
};

struct RadialProlongationRadialRotationManipulatorAndNumOfBites : B {

    const int type_id = 96;
    const int parent_type_id = 35;

    std::string rotation_manipulator;
    std::string height;
    std::string num_of_bites;

    RadialProlongationRadialRotationManipulatorAndNumOfBites()
    {
        _is_obsolete = false;
        _library_name = "↓ → (∠°), H, number of bites";
        _process_name = "↓ → ({}°), {} mm, {} bites";
        _labels = "Rotation around manipulator axis [°]:|Height [mm]:|Number of bites:";
        _deformation_type = "radial_prolongation";
        _speed_column_name = "speed_prolongation";
        _trigger = "keep";
        _sql_query_formula = "rotation_manipulator NUMERIC(6, 3) DEFAULT NULL, "
                             "height NUMERIC(9, 3) DEFAULT NULL, "
                             "num_of_bites SMALLINT DEFAULT NULL";
    }
};

struct RadialProlongationRadial3RotationsAndFeed : B {

    const int type_id = 80;
    const int parent_type_id = 35;

    std::string x_rotation;
    std::string y_rotation;
    std::string z_rotation;
    std::string height;
    std::string radial_feed;

    RadialProlongationRadial3RotationsAndFeed()

    {        _is_obsolete = true;
        _library_name = "rotation axes (manipulator, operator, press)=(∠°,∠°,∠°), ↓ H, feed";
        _process_name = "rotation axes (manipulator, operator, press)=({}°,{}°,{}°), ↓ {} mm, feed {} mm";
        _labels = "Rotation around manipulator axis [°]:|Rotation around operator view axis [°]:|Rotation around press (vertical) axis [°]:|Height [mm]:|Feed [mm]:";
        _deformation_type = "radial_prolongation";
        _speed_column_name = "speed_prolongation";
        _trigger = "keep";
        _sql_query_formula = " x_rotation NUMERIC(6, 3) DEFAULT NULL, "
                             "y_rotation NUMERIC(6, 3) DEFAULT NULL, "
                             "z_rotation NUMERIC(6, 3) DEFAULT NULL, "
                             "height NUMERIC(9, 3) DEFAULT NULL, "
                             "radial_feed NUMERIC(9, 3) DEFAULT NULL";
    }
};

struct RadialProlongationRadial3RotationsAndNumOfBites : B {

    const int type_id = 82;
    const int parent_type_id = 35;


    std::string x_rotation;
    std::string y_rotation;
    std::string z_rotation;
    std::string height;
    std::string num_of_bites;

    RadialProlongationRadial3RotationsAndNumOfBites()
    {
        _is_obsolete = true;
        _library_name = "rotation axes (manipulator, operator, press)=(∠°,∠°,∠°), ↓ H, number of bites";
        _process_name = "rotation axes (manipulator, operator, press)=({}°,{}°,{}°), ↓ {} mm, {} bites";
        _labels = "Rotation around manipulator axis [°]:|Rotation around operator view axis [°]:|Rotation around press (vertical) axis [°]:|Height [mm]:|Number of bites:";
        _deformation_type = "radial_prolongation";
        _speed_column_name = "speed_prolongation";
        _trigger = "keep";
        _sql_query_formula = " x_rotation NUMERIC(6, 3) DEFAULT NULL, "
                             "y_rotation NUMERIC(6, 3) DEFAULT NULL, "
                             "z_rotation NUMERIC(6, 3) DEFAULT NULL, "
                             "height NUMERIC(9, 3) DEFAULT NULL, "
                             "num_of_bites SMALLINT DEFAULT NULL";
    }
};

struct RadialProlongationUpSideDownSimple : B {

    const int type_id = 81;
    const int parent_type_id = 35;


    std::string axial_rotation;
    std::string height;
    std::string radial_feed;

    RadialProlongationUpSideDownSimple()
    {
        _is_obsolete = true;
        _library_name = "up side down, axial ∠°, ↓ H, feed";
        _process_name = "up side down, axial {}°, ↓ {} mm, feed {} mm";
        _labels = "Rotation around original billet axis [°]:|Height of radial prolongation [mm]:|Radial feed [mm]:";
        _deformation_type = "radial_prolongation";
        _speed_column_name = "speed_prolongation";
        _trigger = "keep";
        _sql_query_formula = "axial_rotation NUMERIC(6, 3) DEFAULT NULL, "
                             "height NUMERIC(9, 3) DEFAULT NULL, "
                             "radial_feed NUMERIC(9, 3) DEFAULT NULL";
    }
};

struct FullDie : B {

    const int type_id = 39;
    const int parent_type_id = 36;

    FullDie()
    {
        _is_obsolete = false;
        _library_name = "full die";
        _process_name = "full die";
        _labels = "";
        _deformation_type = "";
        _speed_column_name = "";
        _trigger = "keep";
        _sql_query_formula = "";
    }
};

struct RadialForgingGfm : B {

    const int type_id = 63;
    const int parent_type_id = 36;

    RadialForgingGfm()
    {
        _is_obsolete = false;
        _library_name = "radial forging (GFM)";
        _process_name = "GFM";
        _labels = "";
        _deformation_type = "";
        _speed_column_name = "";
        _trigger = "keep";
        _sql_query_formula = "";
    }
};

struct RadialForgingGfmRoundingSpiral1Rotation1Feed : B {

    const int type_id = 64;
    const int parent_type_id = 63;


    std::string diameter;
    std::string rotation_per_bite;
    std::string feed;

    RadialForgingGfmRoundingSpiral1Rotation1Feed()
    {
        _is_obsolete = false;
        _library_name = "⌀D, spiral rounding 1 rotation per 1 feed";
        _process_name = "⌀{}, spiral rounding 1 rotation {}° per 1 feed {} mm";
        _labels = "Final diameter [mm]:|Rotation [°]:|Feed [mm]:";
        _deformation_type = "axial_prolongation";
        _speed_column_name = "speed_prolongation";
        _trigger = "keep";
        _sql_query_formula = "diameter NUMERIC(9, 3) DEFAULT NULL, "
                             "rotation_per_bite NUMERIC(6, 3) DEFAULT NULL, "
                             "feed NUMERIC(9, 3) DEFAULT NULL";
    }
};

struct FullDieSimple : B {

    const int type_id = 52;
    const int parent_type_id = 39;


    std::string rotation;
    std::string height;

    FullDieSimple()
    {
        _is_obsolete = false;
        _library_name = "↓_↓ (∠°) H";
        _process_name = "↓_↓ ({}°) {}";
        _labels = "Rotation before full-die bite [°]:|Height of bite [mm]:";
        _deformation_type = "full_die";
        _speed_column_name = "speed_full_die";
        _trigger = "keep";
        _sql_query_formula = "rotation NUMERIC(6, 3) DEFAULT NULL, "
                             "height NUMERIC(9, 3) DEFAULT NULL";
    }
};

struct FullDieFindDiagonal : B {

    const int type_id = 53;
    const int parent_type_id = 39;


    std::string rotation;
    std::string height;

    FullDieFindDiagonal()
    {
        _is_obsolete = true;
        _library_name = "(∠°) H, start from diagonal (adjust rotation angle)";
        _process_name = "({}°) {}, start from diagonal (adjust rotation angle)";
        _labels = "Rotation before full-die bite [°]:|Height of bite [mm]:";
        _deformation_type = "full_die";
        _speed_column_name = "speed_full_die";
        _trigger = "keep";
        _sql_query_formula = "rotation NUMERIC(6, 3) DEFAULT NULL, "
                             "height NUMERIC(9, 3) DEFAULT NULL";
    }
};

struct FullDieStartFromHeight : B {

    const int type_id = 54;
    const int parent_type_id = 39;

    std::string height;

    FullDieStartFromHeight()
    {
        _is_obsolete = true;
        _library_name = "(0...90°) H, rotate and start from highest side of rectangular section";
        _process_name = "(0...90°) {}, rotate and start from highest side of rectangular section";
        _labels = "Height of bite [mm]:";
        _deformation_type = "full_die";
        _speed_column_name = "speed_full_die";
        _trigger = "keep";
        _sql_query_formula = "height NUMERIC(9, 3) DEFAULT NULL";
    }
};

struct FullDieRounding : B {

    const int type_id = 55;
    const int parent_type_id = 39;

    std::string diameter;

    FullDieRounding()
    {
        _is_obsolete = true;
        _library_name = "⌀D, full-die multiple bite rounding";
        _process_name = "⌀{}, full-die multiple bite rounding";
        _labels = "Diameter of full-die rounding [mm]";
        _deformation_type = "full_die";
        _speed_column_name = "speed_full_die";
        _trigger = "keep";
        _sql_query_formula = "diameter NUMERIC(9, 3) DEFAULT NULL";
    }
};

struct HotCut : B {

    const int type_id = 40;
    const int parent_type_id = 36;

    HotCut()
    {
        _is_obsolete = false;
        _library_name = "hot cutting";
        _process_name = "hot cutting";
        _labels = "";
        _deformation_type = "";
        _speed_column_name = "";
        _trigger = "keep";
        _sql_query_formula = "";
    }
};

struct HotCutKeepLength : B {

    const int type_id = 56;
    const int parent_type_id = 40;


    std::string pieces_count;
    std::string piece_number;
    std::string length_to_keep;

    HotCutKeepLength()
    {
        _is_obsolete = true;
        _library_name = "Cut on few pieces, keep length";
        _process_name = "Cut on {} pieces, keep piece #{} with length {} mm";
        _labels = "Pieces count:|Keep piece #:|Keep length [mm]:";
        _deformation_type = "hot_cutting";
        _speed_column_name = "speed_prolongation";
        _trigger = "keep";
        _sql_query_formula = "pieces_count SMALLINT NOT NULL DEFAULT 2, "
                             "piece_number SMALLINT NOT NULL DEFAULT 1, "
                             "length_to_keep NUMERIC(9, 3) DEFAULT NULL";
    }
};

struct HotCutKeepPercentage : B {

    const int type_id = 57;
    const int parent_type_id = 40;


    std::string pieces_count;
    std::string piece_number;
    std::string percentage_to_keep;

    HotCutKeepPercentage()
    {
        _is_obsolete = false;
        _library_name = "Cut on few pieces, keep % of length";
        _process_name = "Cut on {} pieces, keep piece #{} with length ratio {}%";
        _labels = "Pieces count:|Keep piece #:|Keep length ratio [%]:";
        _deformation_type = "hot_cutting";
        _speed_column_name = "speed_prolongation";
        _trigger = "keep";
        _sql_query_formula = "pieces_count SMALLINT NOT NULL DEFAULT 2, "
                             "piece_number SMALLINT NOT NULL DEFAULT 1, "
                             "percentage_to_keep NUMERIC(6, 3) DEFAULT NULL";
    }
};

struct HotCutCutOffLengthKeepRest : B {

    const int type_id = 58;
    const int parent_type_id = 40;


    std::string pieces_count;
    std::string piece_number;
    std::string cut_off_length;

    HotCutCutOffLengthKeepRest()
    {
        _is_obsolete = true;
        _library_name = "Cut on few pieces, remove pieces with total length, keep residual piece";
        _process_name = "Cut on {} pieces, remove pieces with total length {} mm, keep residual piece #{}";
        _labels = "Keep side:|Cut off length [mm]:";
        _deformation_type = "hot_cutting";
        _speed_column_name = "speed_prolongation";
        _trigger = "keep";
        _sql_query_formula = "pieces_count SMALLINT NOT NULL DEFAULT 2, "
                             "piece_number SMALLINT NOT NULL DEFAULT 1, "
                             "cut_off_length NUMERIC(9, 3) DEFAULT NULL";
    }
};

struct HotCutOffsetLength : B {

    const int type_id = 59;
    const int parent_type_id = 40;


    std::string tail_offset_length;
    std::string ingot_side_id;
    std::string length_to_keep;

    // Foreign key helper methods (stubs)
    static std::string ingot_side(std::size_t id) {
        // TODO: Call corresponding class or service to resolve 'name' by 'id'.
        return {};
    }

    HotCutOffsetLength()
    {
        _is_obsolete = true;
        _library_name = "Offset from tail and keep a piece of length";
        _process_name = "Offset {} mm at {} side tail and keep length {} mm";
        _labels = "Offset from tail [mm]:|Select ingot side:|Keep length after offset [mm]:";
        _deformation_type = "hot_cutting";
        _speed_column_name = "speed_prolongation";
        _trigger = "keep";
        _sql_query_formula = "ingot_side_id INT NOT NULL DEFAULT 1, "
                             "tail_offset_length NUMERIC(9, 3) DEFAULT NULL, "
                             "length_to_keep NUMERIC(9, 3) DEFAULT NULL, "
                             "FOREIGN KEY (ingot_side_id) REFERENCES ingot_side(id) ON DELETE SET DEFAULT";
    }
};

struct HotCutOffsetPercentage : B {

    const int type_id = 60;
    const int parent_type_id = 40;


    std::string tail_offset_percentage;
    std::string ingot_side_id;
    std::string percentage_to_keep;

    // Foreign key helper methods (stubs)
    static std::string ingot_side(std::size_t id) {
        // TODO: Call corresponding class or service to resolve 'name' by 'id'.
        return {};
    }

    HotCutOffsetPercentage()
    {
        _is_obsolete = true;
        _library_name = "Offset % from tail and keep % of length";
        _process_name = "Offset {}% at {} side tail and keep {}% of length";
        _labels = "Offset from tail [%]:|Select ingot side:|Keep length after offset [%]:";
        _deformation_type = "hot_cutting";
        _speed_column_name = "speed_prolongation";
        _trigger = "keep";
        _sql_query_formula = "ingot_side_id INT NOT NULL DEFAULT 1, "
                             "tail_offset_percentage NUMERIC(6, 3) DEFAULT NULL, "
                             "percentage_to_keep NUMERIC(6, 3) DEFAULT NULL, "
                             "FOREIGN KEY (ingot_side_id) REFERENCES ingot_side(id) ON DELETE SET DEFAULT";
    }
};

struct ColdSawing : B {

    const int type_id = 61;
    const int parent_type_id = 36;

    ColdSawing()
    {
        _is_obsolete = false;
        _library_name = "cold sawing";
        _process_name = "cold sawing";
        _labels = "";
        _deformation_type = "";
        _speed_column_name = "";
        _trigger = "keep";
        _sql_query_formula = "";
    }
};

struct ColdSawingKeepLength : B {

    const int type_id = 85;
    const int parent_type_id = 61;


    std::string pieces_count;
    std::string piece_number;
    std::string length_to_keep;

    ColdSawingKeepLength()
    {
        _is_obsolete = true;
        _library_name = "Saw on few pieces, keep length";
        _process_name = "Saw on {} pieces, keep piece #{} with length {} mm";
        _labels = "Pieces count:|Keep piece #:|Keep length [mm]:";
        _deformation_type = "cold_sawing";
        _speed_column_name = "speed_prolongation";
        _trigger = "keep";
        _sql_query_formula = "pieces_count SMALLINT NOT NULL DEFAULT 2, "
                             "piece_number SMALLINT NOT NULL DEFAULT 1, "
                             "length_to_keep NUMERIC(9, 3) DEFAULT NULL";
    }
};

struct ColdSawingKeepPercentage : B {

    const int type_id = 86;
    const int parent_type_id = 61;


    std::string pieces_count;
    std::string piece_number;
    std::string percentage_to_keep;

    ColdSawingKeepPercentage()
    {
        _is_obsolete = false;
        _library_name = "Saw on few pieces, keep % of length";
        _process_name = "Saw on {} pieces, keep piece #{} with length ratio {}%";
        _labels = "Pieces count:|Keep piece #:|Keep length ratio [%]:";
        _deformation_type = "cold_sawing";
        _speed_column_name = "speed_prolongation";
        _trigger = "keep";
        _sql_query_formula = "pieces_count SMALLINT NOT NULL DEFAULT 2, "
                             "piece_number SMALLINT NOT NULL DEFAULT 1, "
                             "percentage_to_keep NUMERIC(6, 3) DEFAULT NULL";
    }
};

struct SurfaceTreatment : B {

    const int type_id = 66;
    const int parent_type_id = 36;

    SurfaceTreatment()
    {
        _is_obsolete = true;
        _library_name = "surface treatment";
        _process_name = "surface treatment";
        _labels = "";
        _deformation_type = "";
        _speed_column_name = "";
        _trigger = "keep";
        _sql_query_formula = "";
    }
};

struct SurfaceTreatmentInsulationCoating : B {

    const int type_id = 67;
    const int parent_type_id = 66;


    std::string insulation_area_id;
    std::string insulation_material_id;

    // Foreign key helper methods (stubs)
    static std::string insulation_area(int16_t insulation_area_id) {
        // TODO: Call corresponding class or service to resolve 'insulation_area_name' by 'insulation_area_id'.
        return {};
    }
    static std::string insulation_material(int16_t insulation_material_id) {
        // TODO: Call corresponding class or service to resolve 'insulation_material_name' by 'insulation_material_id'.
        return {};
    }

    SurfaceTreatmentInsulationCoating()
    {
        _is_obsolete = true;
        _library_name = "Surface insulation";
        _process_name = "Surface insulation {} with {}";
        _labels = "Insulation area:|Insulation material:";
        _deformation_type = "cold_sawing";
        _speed_column_name = "speed_prolongation";
        _trigger = "keep";
        _sql_query_formula = "insulation_area_id SMALLINT NOT NULL DEFAULT 1, "
                             "insulation_material_id SMALLINT NOT NULL DEFAULT 1, "
                             "FOREIGN KEY (insulation_area_id) REFERENCES insulation_area(insulation_area_id) ON DELETE RESTRICT, "
                             "FOREIGN KEY (insulation_material_id) REFERENCES insulation_material(insulation_material_id) ON DELETE RESTRICT";
    }
};

struct ColdSawingCutOffLengthKeepRest : B {

    const int type_id = 87;
    const int parent_type_id = 61;


    std::string pieces_count;
    std::string piece_number;
    std::string cut_off_length;

    ColdSawingCutOffLengthKeepRest()
    {
        _is_obsolete = true;
        _library_name = "Saw on few pieces, remove pieces with total length, keep residual piece";
        _process_name = "Saw on {} pieces, remove pieces with total length {} mm, keep residual piece #{}";
        _labels = "Keep side:|Cut off length [mm]:";
        _deformation_type = "cold_sawing";
        _speed_column_name = "speed_prolongation";
        _trigger = "keep";
        _sql_query_formula = "pieces_count SMALLINT NOT NULL DEFAULT 2, "
                             "piece_number SMALLINT NOT NULL DEFAULT 1, "
                             "cut_off_length NUMERIC(9, 3) DEFAULT NULL";
    }
};

struct ColdSawingOffsetLength : B {

    const int type_id = 88;
    const int parent_type_id = 61;

    std::string tail_offset_length;
    std::string ingot_side_id;
    std::string length_to_keep;

    // Foreign key helper methods (stubs)
    static std::string ingot_side(std::size_t id) {
        // TODO: Call corresponding class or service to resolve 'name' by 'id'.
        return {};
    }

    ColdSawingOffsetLength()
    {
        _is_obsolete = true;
        _library_name = "Offset from tail and keep a piece of length";
        _process_name = "Offset {} mm at {} side tail and keep length {} mm";
        _labels = "Offset from tail [mm]:|Select ingot side:|Keep length after offset [mm]:";
        _deformation_type = "cold_sawing";
        _speed_column_name = "speed_prolongation";
        _trigger = "keep";
        _sql_query_formula = "ingot_side_id INT NOT NULL DEFAULT 1, "
                             "tail_offset_length NUMERIC(9, 3) DEFAULT NULL, "
                             "length_to_keep NUMERIC(9, 3) DEFAULT NULL, "
                             "FOREIGN KEY (ingot_side_id) REFERENCES ingot_side(id) ON DELETE SET DEFAULT";
    }
};

struct ColdSawingOffsetPercentage : B {

    const int type_id = 89;
    const int parent_type_id = 61;

    std::string tail_offset_percentage;
    std::string ingot_side_id;
    std::string percentage_to_keep;

    // Foreign key helper methods (stubs)
    static std::string ingot_side(std::size_t id) {
        // TODO: Call corresponding class or service to resolve 'name' by 'id'.
        return {};
    }

    ColdSawingOffsetPercentage()
    {
        _is_obsolete = true;
        _library_name = "Offset % from tail and keep % of length";
        _process_name = "Offset {}% at {} side tail and keep {}% of length";
        _labels = "Offset from tail [%]:|Select ingot side:|Keep length after offset [%]:";
        _deformation_type = "cold_sawing";
        _speed_column_name = "speed_prolongation";
        _trigger = "keep";
        _sql_query_formula = "ingot_side_id INT NOT NULL DEFAULT 1, "
                             "tail_offset_percentage NUMERIC(6, 3) DEFAULT NULL, "
                             "percentage_to_keep NUMERIC(6, 3) DEFAULT NULL, "
                             "FOREIGN KEY (ingot_side_id) REFERENCES ingot_side(id) ON DELETE SET DEFAULT";
    }
};

} // namespace Operations