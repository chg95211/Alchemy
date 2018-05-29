#include <alchemy.h>

using namespace alchemy;
using namespace std;

int main()
{
    MnistLoader<float> train_loader("/home/ffiirree/Code/Alchemy/resources/mnist/train-images.idx3-ubyte",
                                    "/home/ffiirree/Code/Alchemy/resources/mnist/train-labels.idx1-ubyte");

    MnistLoader<float> test_loader("/home/ffiirree/Code/Alchemy/resources/mnist/t10k-images.idx3-ubyte",
                                   "/home/ffiirree/Code/Alchemy/resources/mnist/t10k-labels.idx1-ubyte");

    vector<LayerParameter> params = {
            LayerParameter()
                    .name("train input layer")
                    .type(INPUT_LAYER)
                    .phase(TRAIN)
                    .output("data")
                    .output("label")
                    .input_param(
                            InputParameter()
                                    .source(&train_loader)
                                    .batch_size(64)
                                    .scale(1./255)
                    ),
            LayerParameter()
                    .name("simples input layer")
                    .type(INPUT_LAYER)
                    .phase(TEST)
                    .output("data")
                    .output("label")
                    .input_param(
                            InputParameter()
                                    .source(&test_loader)
                                    .batch_size(100)
                                    .scale(1./255)
                    ),
            LayerParameter()
                    .name("ip layer 1")
                    .type(INNER_PRODUCT_LAYER)
                    .input("data")
                    .output("ip1")
                    .ip_param(
                            InnerProductParameter()
                                    .output_size(30)
                                    .wlr(0.075)
                                    .blr(0.1)
                                    .weight_decay(0.005)
                                    .weight_filler(XAVIER)
                                    .bias_filler(XAVIER)
                    ),
            LayerParameter()
                    .name("relu layer 1")
                    .type(RELU_LAYER)
                    .input("ip1")
                    .output("r1")
                    .relu_param(
                            ReLuParameter()
                                    .alpha(-0.1)
                    ),
            LayerParameter()
                    .name("ip layer 2")
                    .type(INNER_PRODUCT_LAYER)
                    .input("r1")
                    .output("ip2")
                    .ip_param(
                            InnerProductParameter()
                                    .output_size(10)
                                    .wlr(0.075)
                                    .blr(0.1)
                                    .weight_decay(0.005)
                                    .weight_filler(XAVIER)
                                    .bias_filler(XAVIER)
                    ),
            LayerParameter()
                    .name("relu layer 2")
                    .type(RELU_LAYER)
                    .input("ip2")
                    .output("r2")
                    .relu_param(
                            ReLuParameter()
                                    .alpha(-0.1)
                    ),
            LayerParameter()
                    .name("eucl layer")
                    .type(EUCLIDEAN_LOSS_LAYER)
                    .phase(TRAIN)
                    .input("r2")
                    .input("label")
                    .output("loss")
                    .euclidean_param(
                            EuclideanLossParameter()
                    ),
            LayerParameter()
                    .name("acc layer")
                    .type(ACCURACY_LAYER)
                    .phase(TEST)
                    .input("r2")
                    .input("label")
                    .output("accuracy")
                    .accuracy_param(
                            AccuracyParameter()
                    )
    };

    auto optimize_param = OptimizerParameter()
            .mode(Global::GPU)
            .max_iter(10000)
            .test_iter(100)
            .test_interval(500)
            .train_net_param(NetworkParameter().layer_params(params))
            .test_net_param(NetworkParameter().layer_params(params));

    SgdOptimizer<GPU, float> o(optimize_param);

    o.optimize();

    return 0;
}