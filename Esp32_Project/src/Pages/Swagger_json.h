const char * swaggerJSON = "{\
    \"swagger\": \"2.0\",\
    \"info\": {\
        \"description\": \"This is a sample server Petstore server.\",\
        \"version\": \"1.0.0\",\
        \"title\": \"IoT application\"\
    },\
    \"host\": \"192.168.137.52\",\
    \"tags\": [\
        {\
            \"name\": \"Temperature\",\
            \"description\": \"Getting temperature measurements\"\
        }\
    ],\
    \"paths\": {\
        \"/temperature\": {\
            \"get\": {\
                \"tags\": [\
                    \"Temperature\"\
                ],\
                \"summary\": \"Endpoint for getting temperature measurements\",\
                \"description\": \"\",\
                \"operationId\": \"getTemperature\",\
                \"responses\": {\
                    \"200\": {\
                        \"description\": \"A list of temperature measurements\",\
                        \"schema\": {\
                            \"$ref\": \"#/definitions/temperatureMeasurement\"\
                        }\
                    }\
                }\
            }\
        }\
    },\
    \"definitions\": {\
        \"temperatureMeasurement\": {\
            \"type\": \"object\",\
            \"properties\": {\
                \"value\": {\
                    \"type\": \"string\"\
                },\
                \"timestamp\": {\
                    \"type\": \"string\"\
                }\
            }\
        }\
    }\
}";