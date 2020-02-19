#include "MessageStream.h"

#define READER_CASE(type) \
    case sizeof(type): \
    { \
        *(type*)field = reader.read<type>(); \
        break; \
    }

#define WRITER_CASE(type) \
    case sizeof(type): \
    { \
        writer.write<type>(*(type*)field); \
        break; \
    }

void MessageStream::readMessages(BitReader& reader, const MessageInfo* info, MemoryPool* pool,
    std::vector<MessageRequest>& requests, std::vector<MessageData>& messages, const Address& address)
{
    if (reader.read())
        requests.emplace_back(info, address);

    if (!reader.read())
        return;

    size_t count = 1;

    if (reader.read())
        count += reader.readVarUInt();

    for (size_t i = 0; i < count; i++)
    {
        std::shared_ptr<Message> message = pool->allocate<Message>();

        for (size_t j = 0; j < info->fieldCount; j++)
        {
            const FieldInfo& fieldInfo = info->fields[j];
            uint8_t* field = (uint8_t*)message.get() + fieldInfo.byteOffset;

            switch (fieldInfo.variance)
            {

            case Variance::INVARIANT:
                switch (fieldInfo.byteSize)
                {
                    READER_CASE(uint8_t);
                    READER_CASE(uint16_t);
                    READER_CASE(uint32_t);
                    READER_CASE(uint64_t);

                default:
                    for (size_t k = 0; k < fieldInfo.byteSize; k++)
                        field[k] = reader.read<uint8_t>();

                    break;
                }

                break;

            case Variance::UNSIGNED_VARIANT:
                *(VarUInt*)field = reader.readVarUInt();
                break;

            case Variance::SIGNED_VARIANT:
                *(VarInt*)field = reader.readVarInt();
                break;

            }
        }

        messages.emplace_back(info, std::move(message), address);
    }
}

void MessageStream::writeMessages(BitWriter& writer, const MessageInfo* info,
    const std::vector<MessageRequest>& requests, const std::vector<MessageData>& messages, const Address& address)
{
    bool requestFound = false;
    for (auto& request : requests)
    {
        if (request.getInfo() != info || request.getAddress() != address)
            continue;

        requestFound = true;
        break;
    }

    writer.write(requestFound);

    size_t count = 0;
    for (auto& messageData : messages)
    {
        if (messageData.getInfo() != info || messageData.getAddress() != address)
            continue;

        count++;
    }

    writer.write(count > 0);
    if (count < 1)
        return;

    writer.write(count > 1);
    if (count > 1)
        writer.writeVarUInt(count - 1);

    for (auto& messageData : messages)
    {
        if (messageData.getInfo() != info || messageData.getAddress() != address)
            continue;

        std::shared_ptr<uint8_t[]> message = messageData.get<uint8_t[]>();

        for (size_t i = 0; i < info->fieldCount; i++)
        {
            const FieldInfo& fieldInfo = info->fields[i];
            uint8_t* field = message.get() + fieldInfo.byteOffset;

            switch (fieldInfo.variance)
            {

            case Variance::INVARIANT:
                switch (fieldInfo.byteSize)
                {
                    WRITER_CASE(uint8_t);
                    WRITER_CASE(uint16_t);
                    WRITER_CASE(uint32_t);
                    WRITER_CASE(uint64_t);

                default:
                    for (size_t j = 0; j < fieldInfo.byteSize; j++)
                        writer.write<uint8_t>(field[j]);

                    break;

                }
                break;

            case Variance::UNSIGNED_VARIANT:
                writer.writeVarUInt(*(VarUInt*)field);
                break;

            case Variance::SIGNED_VARIANT:
                writer.writeVarInt(*(VarInt*)field);
                break;

            }
        }
    }
}
