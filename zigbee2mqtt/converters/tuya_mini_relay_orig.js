import * as m from 'zigbee-herdsman-converters/lib/modernExtend';

export default {
    fingerprint: [
                    {manufacturerName: '_TZ3000_kqvb5akv'}
                 ],
    zigbeeModel: ['TS0001'],
    model: 'Original Tuya mini relay ready for update',
    vendor: 'Slacky-DIY',
    description: 'Original Tuya mini relay ready for custom Firmware update',
    extend: [
                    m.identify()
            ],
    meta: {},
    ota: true,
};
