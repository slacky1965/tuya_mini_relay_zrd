import * as m from 'zigbee-herdsman-converters/lib/modernExtend';
import * as exposes_1 from 'zigbee-herdsman-converters/lib/exposes';
import * as reporting from 'zigbee-herdsman-converters/lib/reporting';
import * as utils_1 from 'zigbee-herdsman-converters/lib/utils';
import * as logger from 'zigbee-herdsman-converters/lib/logger';


function localActionExtend(args = {}) {
    const { localAction = ["hold", "single", "double", "triple", "quadruple", "quintuple", "release"], 
            reporting = true, 
            reportingConfig = {"min": 10, "max": 0, change: 1},
            endpointNames = undefined } = args;
    let actions = localAction;
    if (endpointNames) {
        actions = localAction.flatMap((c) => endpointNames.map((e) => `${c}_${e}`));
    }
    const exposes = [exposes_1.presets.enum("action", exposes_1.access.STATE, actions)];
    const attribute = "presentValue";

    
    const actionPayloadLookup = {
      0: "hold",
      1: "single",
      2: "double",
      3: "triple",
      4: "quadruple",
      5: "quintuple",
      255: "release",
    };

    const fromZigbee = [
        {
            cluster: "genMultistateInput",
            type: ["attributeReport", "readResponse"],
            convert: (model, msg, publish, options, meta) => {
                if ((0, utils_1.hasAlreadyProcessedMessage)(msg, model))
                    return;
                const data = msg.data;
                //logger.logger.info('msg.data: ' + data[attribute]);
                if (data[attribute] == 300)
                    return "N/A";
                const payload = { action: (0, utils_1.postfixWithEndpointName)(actionPayloadLookup[data[attribute]], msg, model, meta) };
                return payload;
            },
        },
    ];
    const result = { exposes, fromZigbee, isModernExtend: true };
    if (reporting)
        result.configure = [m.setupConfigureForBinding("genMultistateInput", "input", endpointNames),
                            m.setupConfigureForReporting("genMultistateInput", attribute, {config: reportingConfig, access: exposes_1.access.GET, endpointNames})];
        
    return result;

}

export default {
    zigbeeModel: ['TS0001_power-SlD'],
    model: 'TS0001_power-SlD',
    vendor: 'Slacky-DIY',
    description: 'Automatically generated definition',
    extend: [
        // m.deviceEndpoints({"endpoints":{"1":1}}), 
        m.onOff({"powerOnBehavior":true}), 
        m.commandsOnOff(),
        // localActionExtend({"endpointNames":["1"]}),
        localActionExtend(),
        m.enumLookup({
            name: "switch_actions",
            lookup: {off: 0, on: 1},
            cluster: "genOnOffSwitchCfg",
            attribute: "switchActions",
            description: "Actions switch",
        }),
        m.enumLookup({
            "name": 'switch_type',
            "lookup": {"toggle": 0, "momentary": 1, "multifunction": 2},
            "cluster": 'genOnOffSwitchCfg',
            "attribute": {"ID": 0xf000, "type": 0x30},
            "description": 'Switch 1 type'}),
        m.enumLookup({
            "name": 'operation_mode',
            "lookup": {"control_relay": 0, "decoupled": 1},
            "cluster": 'genOnOffSwitchCfg',
            "attribute": {"ID": 0xf001, "type": 0x30},
            "reporting": {"min": 0, "max": 65000, "change": 0},
            "description": 'Relay decoupled'}),
        m.electricityMeter({
          current: {"divisor": 100}, 
          voltage: {"divisor": 100}, 
          power: {"divisor": 100}, 
          energy: {"divisor": 100},
          acFrequency: {"divisor": 100}
        }), 
        // m.commandsOnOff()
        ],
    meta: {},
    ota: true,
};
